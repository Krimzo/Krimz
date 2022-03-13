#include "Engine/Engine.h"


void Start() {
	// Setting the icon
	win.setIcon("res/icons/k.ico");

	// Maximizing the window
	win.maximize();

	// Creating the gpu
	gpu = new kl::gpu(win.getWND(), true);

	// Resize callback
	win.resize = Resize;

	// Rasters
	solid_ra = gpu->newRasterState(false, true);
	wire_ra = gpu->newRasterState(true, true);
	shadow_ra = gpu->newRasterState(false, true, false);
	gpu->bind(solid_ra);

	// Depth states
	disabled_ds = gpu->newDepthState(false, false, false);
	depth_ds = gpu->newDepthState(true, false, false);
	write_ds = gpu->newDepthState(true, true, false);
	mask_ds = gpu->newDepthState(false, true, true);

	// Shaders
	ID3D11InputLayout* defaultLayout = nullptr;
	editor_vtx = gpu->newVertexShader(kl::file::read("res/shaders/editor.hlsl"), &defaultLayout);
	editor_pxl = gpu->newPixelShader(kl::file::read("res/shaders/editor.hlsl"));
	shadow_vtx = gpu->newVertexShader(kl::file::read("res/shaders/shadows.hlsl"));
	shadow_pxl = gpu->newPixelShader(kl::file::read("res/shaders/shadows.hlsl"));
	index_vtx = gpu->newVertexShader(kl::file::read("res/shaders/index.hlsl"));
	index_pxl = gpu->newPixelShader(kl::file::read("res/shaders/index.hlsl"));
	outline_vtx = gpu->newVertexShader(kl::file::read("res/shaders/outline.hlsl"));
	outline_pxl = gpu->newPixelShader(kl::file::read("res/shaders/outline.hlsl"));
	gizmo_vtx = gpu->newVertexShader(kl::file::read("res/shaders/gizmo.hlsl"));
	gizmo_pxl = gpu->newPixelShader(kl::file::read("res/shaders/gizmo.hlsl"));
	gpu->bind(defaultLayout);

	// Constant buffers
	editor_vtx_cb =	gpu->newConstBuffer(sizeof(DRAW_VS_CB));
	editor_pxl_cb =	gpu->newConstBuffer(sizeof(DRAW_PS_CB));
	shadow_vtx_cb =	gpu->newConstBuffer(sizeof(kl::mat4));
	index_vtx_cb = gpu->newConstBuffer(sizeof(kl::mat4));
	outline_pxl_cb = gpu->newConstBuffer(sizeof(kl::float4));
	gizmo_vtx_cb = gpu->newConstBuffer(sizeof(kl::mat4));
	gizmo_pxl_cb = gpu->newConstBuffer(sizeof(GIZM_PS_CB));

	// Samplers
	gpu->bind(gpu->newSamplerState(true, true), 0);
	D3D11_SAMPLER_DESC shadowSamp = {};
	shadowSamp.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.BorderColor[0] = 1.0f;
	shadowSamp.BorderColor[1] = 1.0f;
	shadowSamp.BorderColor[2] = 1.0f;
	shadowSamp.BorderColor[3] = 1.0f;
	shadowSamp.ComparisonFunc = D3D11_COMPARISON_LESS;
	gpu->bind(gpu->newSamplerState(&shadowSamp), 1);

	// Picking
	D3D11_TEXTURE2D_DESC pickTexDes = {};
	pickTexDes.Width = win.getSize().x;
	pickTexDes.Height = win.getSize().y;
	pickTexDes.MipLevels = 1;
	pickTexDes.ArraySize = 1;
	pickTexDes.Format = DXGI_FORMAT_R32_FLOAT;
	pickTexDes.SampleDesc.Count = 1;
	pickTexDes.Usage = D3D11_USAGE_DEFAULT;
	pickTexDes.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	pickingTex = gpu->newTexture(&pickTexDes);
	pickingTexST = gpu->newTextureST(pickingTex, kl::int2(1));
	pickingTargetV = gpu->newTargetView(pickingTex);

	// Outline
	ID3D11Texture2D* outlineTex = gpu->newTexture(&pickTexDes);
	outlineTargetV = gpu->newTargetView(outlineTex);
	outlineShaderV = gpu->newShaderView(outlineTex);
	gpu->destroy(outlineTex);

	// Camera
	camera.position = kl::float3(-1.4f, 1.25f, -6.0f);
	camera.forward = kl::float3(0.55f, -0.3f, 0.9f);

	// Sun
	D3D11_TEXTURE2D_DESC sunTexDesc = {};
	sunTexDesc.Width = 4096;
	sunTexDesc.Height = 4096;
	sunTexDesc.MipLevels = 1;
	sunTexDesc.ArraySize = 1;
	sunTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	sunTexDesc.SampleDesc.Count = 1;
	sunTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sunTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* sunTex = gpu->newTexture(&sunTexDesc);
	D3D11_DEPTH_STENCIL_VIEW_DESC sunDepthVDesc = {};
	sunDepthVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	sunDepthVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	sun.shadowMapDV = gpu->newDepthView(sunTex, &sunDepthVDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC sunShaderVDesc = {};
	sunShaderVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	sunShaderVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sunShaderVDesc.Texture2D.MipLevels = 1;
	sun.shadowMapSV = gpu->newShaderView(sunTex, &sunShaderVDesc);
	sun.direction = kl::float3(0.575f, -0.75f, 2.0f);
	gpu->destroy(sunTex);

	// Gizmos
	gizmo_scale = gpu->newVertBuffer("res/objects/gizmos/scale.obj");
	gizmo_move = gpu->newVertBuffer("res/objects/gizmos/move.obj");
	gizmo_rotate = gpu->newVertBuffer("res/objects/gizmos/rotate.obj");

	/* DEBUG */
	// Skybox
	kl::skybox* clouds = skyboxes.newInst(new kl::skybox(gpu, "Clouds",
		kl::image("res/textures/skyboxes/clouds/front.jpg"),
		kl::image("res/textures/skyboxes/clouds/back.jpg"),
		kl::image("res/textures/skyboxes/clouds/left.jpg"),
		kl::image("res/textures/skyboxes/clouds/right.jpg"),
		kl::image("res/textures/skyboxes/clouds/top.jpg"),
		kl::image("res/textures/skyboxes/clouds/bottom.jpg")
	));
	kl::skybox* night = skyboxes.newInst(new kl::skybox(gpu, "Night",
		kl::image("res/textures/skyboxes/night/night.jpg")
	));
	skybox = clouds;

	// Mesh
	ID3D11Buffer* cube_mes = gpu->newVertBuffer("res/objects/cube.obj");
	ID3D11Buffer* monke_mes = gpu->newVertBuffer("res/objects/monke.obj");
	ID3D11Buffer* horse_mes = gpu->newVertBuffer("res/objects/horse.obj");

	// Texture
	ID3D11ShaderResourceView* lgray_tex = gpu->newShaderView(gpu->newTexture(kl::image(kl::int2(1), kl::colors::lgray)));
	ID3D11ShaderResourceView* checker_tex = gpu->newShaderView(gpu->newTexture(kl::image("res/textures/checkers.jpg")));
	ID3D11ShaderResourceView* horse_tex = gpu->newShaderView(gpu->newTexture(kl::image("res/textures/horse.jpg")));

	// Entity
	kl::entity* plane = entities.newInst(new kl::entity("Plane", cube_mes, lgray_tex));
	plane->size = kl::float3(45.0f, 0.25f, 45.0f);
	plane->position.y = -2.0f;
	plane->roughness = 0.9f;

	kl::entity* horse1 = entities.newInst(new kl::entity("Horse1", horse_mes, horse_tex));
	kl::entity* horse2 = entities.newInst(new kl::entity("Horse2", horse_mes, horse_tex));
	horse1->size *= 50.0f;
	horse2->size *= 50.0f;
	horse1->position.y = -1.8f;
	horse2->position.y = -1.8f;
	horse1->position.x =  15.0f;
	horse2->position.x = -15.0f;
	horse2->rotation.y = 180.0f;

	const int size = 3;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			const int i = y * size + x;
			kl::entity* temp = entities.newInst(new kl::entity("Monke" + std::to_string(i), monke_mes, checker_tex));
			temp->position = kl::float3((x - 1.0f) * 2.0f, y * 2.0f, y * 2.0f);
		}
	}
}
