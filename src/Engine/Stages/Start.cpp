#include "Engine/Engine.h"


void Engine::Stage::Start() {
	// Setting the icon
	Engine::Window::win.setIcon("res/icons/k.ico");

	// Maximizing the window
	Engine::Window::win.maximize();
	Engine::Window::size = Engine::Window::win.getSize();

	// Creating the gpu
	Engine::Render::gpu = new kl::gpu(Engine::Window::win.getWND(), true);

	// Resize callback
	Engine::Window::win.resize = Engine::Stage::Resize;

	// Rasters
	Engine::Rasters::solid = Engine::Render::gpu->newRasterState(false, true);
	Engine::Rasters::wire = Engine::Render::gpu->newRasterState(true, true);
	Engine::Rasters::shadow = Engine::Render::gpu->newRasterState(false, true, false);
	Engine::Render::gpu->bind(Engine::Rasters::solid);

	// Depth states
	Engine::DepthStencil::disabled = Engine::Render::gpu->newDepthState(false, false, false);
	Engine::DepthStencil::depth = Engine::Render::gpu->newDepthState(true, false, false);
	Engine::DepthStencil::write = Engine::Render::gpu->newDepthState(true, true, false);
	Engine::DepthStencil::mask = Engine::Render::gpu->newDepthState(false, true, true);

	// Shaders
	ID3D11InputLayout* defaultLayout = nullptr;
	Engine::Shaders::Vertex::editor = Engine::Render::gpu->newVertexShader(kl::file::read("res/shaders/editor.hlsl"), &defaultLayout);
	Engine::Shaders::Pixel::editor = Engine::Render::gpu->newPixelShader(kl::file::read("res/shaders/editor.hlsl"));
	Engine::Shaders::Vertex::shadow = Engine::Render::gpu->newVertexShader(kl::file::read("res/shaders/shadows.hlsl"));
	Engine::Shaders::Pixel::shadow = Engine::Render::gpu->newPixelShader(kl::file::read("res/shaders/shadows.hlsl"));
	Engine::Shaders::Vertex::index = Engine::Render::gpu->newVertexShader(kl::file::read("res/shaders/index.hlsl"));
	Engine::Shaders::Pixel::index = Engine::Render::gpu->newPixelShader(kl::file::read("res/shaders/index.hlsl"));
	Engine::Shaders::Vertex::outline = Engine::Render::gpu->newVertexShader(kl::file::read("res/shaders/outline.hlsl"));
	Engine::Shaders::Pixel::outline = Engine::Render::gpu->newPixelShader(kl::file::read("res/shaders/outline.hlsl"));
	Engine::Shaders::Vertex::gizmo = Engine::Render::gpu->newVertexShader(kl::file::read("res/shaders/gizmo.hlsl"));
	Engine::Shaders::Pixel::gizmo = Engine::Render::gpu->newPixelShader(kl::file::read("res/shaders/gizmo.hlsl"));
	Engine::Render::gpu->bind(defaultLayout);

	// Constant buffers
	Engine::CBuffers::Vertex::editor = Engine::Render::gpu->newConstBuffer(sizeof(Engine::Struct::DRAW_VS_CB));
	Engine::CBuffers::Pixel::editor = Engine::Render::gpu->newConstBuffer(sizeof(Engine::Struct::DRAW_PS_CB));
	Engine::CBuffers::Vertex::shadow = Engine::Render::gpu->newConstBuffer(sizeof(kl::mat4));
	Engine::CBuffers::Vertex::index = Engine::Render::gpu->newConstBuffer(sizeof(kl::mat4));
	Engine::CBuffers::Pixel::outline = Engine::Render::gpu->newConstBuffer(sizeof(kl::float4));
	Engine::CBuffers::Vertex::gizmo = Engine::Render::gpu->newConstBuffer(sizeof(kl::mat4));
	Engine::CBuffers::Pixel::gizmo = Engine::Render::gpu->newConstBuffer(sizeof(Engine::Struct::GIZM_PS_CB));

	// Samplers
	Engine::Render::gpu->bind(Engine::Render::gpu->newSamplerState(true, true), 0);
	D3D11_SAMPLER_DESC shadowSamp = {};
	shadowSamp.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.BorderColor[0] = 1.0f;
	shadowSamp.BorderColor[1] = 1.0f;
	shadowSamp.BorderColor[2] = 1.0f;
	shadowSamp.BorderColor[3] = 1.0f;
	shadowSamp.ComparisonFunc = D3D11_COMPARISON_LESS;
	Engine::Render::gpu->bind(Engine::Render::gpu->newSamplerState(&shadowSamp), 1);

	// Picking
	D3D11_TEXTURE2D_DESC pickTexDes = {};
	pickTexDes.Width = Engine::Window::win.getSize().x;
	pickTexDes.Height = Engine::Window::win.getSize().y;
	pickTexDes.MipLevels = 1;
	pickTexDes.ArraySize = 1;
	pickTexDes.Format = DXGI_FORMAT_R32_FLOAT;
	pickTexDes.SampleDesc.Count = 1;
	pickTexDes.Usage = D3D11_USAGE_DEFAULT;
	pickTexDes.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Engine::Picking::texture = Engine::Render::gpu->newTexture(&pickTexDes);
	Engine::Picking::textureStag = Engine::Render::gpu->newTextureST(Engine::Picking::texture, kl::int2(1));
	Engine::Picking::targetV = Engine::Render::gpu->newTargetView(Engine::Picking::texture);

	// Outline
	ID3D11Texture2D* outlineTex = Engine::Render::gpu->newTexture(&pickTexDes);
	Engine::Outline::targetV = Engine::Render::gpu->newTargetView(outlineTex);
	Engine::Outline::shaderV = Engine::Render::gpu->newShaderView(outlineTex);
	Engine::Render::gpu->destroy(outlineTex);

	// Camera
	Engine::Render::camera.position = kl::float3(-1.4f, 1.25f, -6.0f);
	Engine::Render::camera.forward = kl::float3(0.55f, -0.3f, 0.9f);

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
	ID3D11Texture2D* sunTex = Engine::Render::gpu->newTexture(&sunTexDesc);
	D3D11_DEPTH_STENCIL_VIEW_DESC sunDepthVDesc = {};
	sunDepthVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	sunDepthVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Engine::Light::sun.shadowMapDV = Engine::Render::gpu->newDepthView(sunTex, &sunDepthVDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC sunShaderVDesc = {};
	sunShaderVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	sunShaderVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sunShaderVDesc.Texture2D.MipLevels = 1;
	Engine::Light::sun.shadowMapSV = Engine::Render::gpu->newShaderView(sunTex, &sunShaderVDesc);
	Engine::Light::sun.direction = kl::float3(0.575f, -0.75f, 2.0f);
	Engine::Render::gpu->destroy(sunTex);

	// Gizmos
	Engine::Gizmo::scaleM = Engine::Render::gpu->newVertBuffer("res/objects/gizmos/scale.obj");
	Engine::Gizmo::moveM = Engine::Render::gpu->newVertBuffer("res/objects/gizmos/move.obj");
	Engine::Gizmo::rotateM = Engine::Render::gpu->newVertBuffer("res/objects/gizmos/rotate.obj");

	/* DEBUG */
	// Skybox
	kl::skybox* clouds = Engine::Background::skyboxes.newInst(new kl::skybox(Engine::Render::gpu, "Clouds",
		kl::image("res/textures/skyboxes/clouds/front.jpg"),
		kl::image("res/textures/skyboxes/clouds/back.jpg"),
		kl::image("res/textures/skyboxes/clouds/left.jpg"),
		kl::image("res/textures/skyboxes/clouds/right.jpg"),
		kl::image("res/textures/skyboxes/clouds/top.jpg"),
		kl::image("res/textures/skyboxes/clouds/bottom.jpg")
	));
	kl::skybox* night = Engine::Background::skyboxes.newInst(new kl::skybox(Engine::Render::gpu, "Night",
		kl::image("res/textures/skyboxes/night/night.jpg")
	));
	Engine::Background::skybox = clouds;

	// Mesh
	ID3D11Buffer* cube_mes = Engine::Render::gpu->newVertBuffer("res/objects/cube.obj");
	ID3D11Buffer* monke_mes = Engine::Render::gpu->newVertBuffer("res/objects/monke.obj");
	ID3D11Buffer* horse_mes = Engine::Render::gpu->newVertBuffer("res/objects/horse.obj");

	// Texture
	ID3D11ShaderResourceView* lgray_tex = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image(kl::int2(1), kl::colors::lgray)));
	ID3D11ShaderResourceView* checker_tex = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("res/textures/checkers.jpg")));
	ID3D11ShaderResourceView* horse_tex = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("res/textures/horse.jpg")));

	// Entity
	kl::entity* plane = Engine::Game::entities.newInst(new kl::entity("Plane", cube_mes, lgray_tex));
	plane->size = kl::float3(45.0f, 0.25f, 45.0f);
	plane->position.y = -2.0f;
	plane->roughness = 0.9f;

	kl::entity* horse1 = Engine::Game::entities.newInst(new kl::entity("Horse1", horse_mes, horse_tex));
	kl::entity* horse2 = Engine::Game::entities.newInst(new kl::entity("Horse2", horse_mes, horse_tex));
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
			kl::entity* temp = Engine::Game::entities.newInst(
				new kl::entity("Monke" + std::to_string(i), monke_mes, checker_tex));
			temp->position = kl::float3((x - 1.0f) * 2.0f, y * 2.0f, y * 2.0f);
		}
	}
}
