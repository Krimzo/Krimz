#include "Stage/Stage.h"
#include "Utility/Window.h"
#include "Render/Render.h"
#include "Utility/Struct.h"
#include "Input/Picking.h"
#include "View/Outline.h"
#include "View/Light.h"
#include "View/Background.h"
#include "Input/Gizmo.h"
#include "GUI/GUI.h"
#include "Scripting/Scripting.h"
#include "Data/Meshes.h"
#include "Data/Textures.h"
#include "Data/Entities.h"


void Engine::Stage::Start() {
	// Setting the icon
	Engine::win.setIcon("resource/icons/k.ico");

	// Maximizing the window
	Engine::win.maximize();

	// Creating the gpu
	Engine::Render::gpu = new kl::gpu(Engine::win.getWND(), true);

	// Resize callback
	Engine::win.resize = Engine::Stage::Resize;

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
	Engine::Shaders::Vertex::editor = Engine::Render::gpu->newVertexShader(kl::file::read("resource/shaders/editor.hlsl"), &defaultLayout);
	Engine::Shaders::Pixel::editor = Engine::Render::gpu->newPixelShader(kl::file::read("resource/shaders/editor.hlsl"));
	Engine::Shaders::Vertex::shadow = Engine::Render::gpu->newVertexShader(kl::file::read("resource/shaders/shadows.hlsl"));
	Engine::Shaders::Pixel::shadow = Engine::Render::gpu->newPixelShader(kl::file::read("resource/shaders/shadows.hlsl"));
	Engine::Shaders::Vertex::index = Engine::Render::gpu->newVertexShader(kl::file::read("resource/shaders/index.hlsl"));
	Engine::Shaders::Pixel::index = Engine::Render::gpu->newPixelShader(kl::file::read("resource/shaders/index.hlsl"));
	Engine::Shaders::Vertex::outline = Engine::Render::gpu->newVertexShader(kl::file::read("resource/shaders/outline.hlsl"));
	Engine::Shaders::Pixel::outline = Engine::Render::gpu->newPixelShader(kl::file::read("resource/shaders/outline.hlsl"));
	Engine::Shaders::Vertex::gizmo = Engine::Render::gpu->newVertexShader(kl::file::read("resource/shaders/gizmo.hlsl"));
	Engine::Shaders::Pixel::gizmo = Engine::Render::gpu->newPixelShader(kl::file::read("resource/shaders/gizmo.hlsl"));
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
	pickTexDes.Width = Engine::win.getSize().x;
	pickTexDes.Height = Engine::win.getSize().y;
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
	Engine::Render::camera.far = 500.0f;
	Engine::Render::camera.shadows = 100.0f;
	Engine::Render::camera.position = kl::float3(-1.4f, 1.25f, -6.0f);
	Engine::Render::camera.forward = kl::float3(0.55f, -0.3f, 0.9f);

	// Sun
	Engine::Light::sun.genBuff(Engine::Render::gpu, 4096);
	Engine::Light::sun.direction = kl::float3(0.575f, -0.75f, 2.0f);

	// Gizmos
	Engine::Gizmo::scaleM = Engine::Render::gpu->newVertBuffer("resource/objects/scale.obj");
	Engine::Gizmo::moveM = Engine::Render::gpu->newVertBuffer("resource/objects/move.obj");
	Engine::Gizmo::rotateM = Engine::Render::gpu->newVertBuffer("resource/objects/rotate.obj");

	// Loading default
	Engine::Default::mesh = new Engine::Mesh("default", Engine::Render::gpu->newVertBuffer("resource/objects/cube.obj"));
	ID3D11Texture2D* defTex = Engine::Render::gpu->newTexture(kl::image(kl::int2(1), kl::colors::magenta));
	Engine::Default::texture = new Engine::Texture("default", Engine::Render::gpu->newShaderView(defTex));
	Engine::Render::gpu->destroy(defTex);

	// GUI icons
	Engine::GUI::folderIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/folder.png").flipV()));
	Engine::GUI::folderEIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/folder_empty.png").flipV()));
	Engine::GUI::fileIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/file.png").flipV()));
	Engine::GUI::imageIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/image.png").flipV()));
	Engine::GUI::objectIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/object.png").flipV()));
	Engine::GUI::codeIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/code.png").flipV()));
	Engine::GUI::scriptIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/script.png").flipV()));

	// Console fix
	kl::console::hide();
}
