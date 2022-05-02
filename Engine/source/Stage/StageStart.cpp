#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Scripting/Scripting.h"
#include "Data/Meshes.h"
#include "Data/Textures.h"
#include "Data/Entities.h"
#include "GUI/GUI.h"


void Engine::Stage::Start() {
	// Setting the icon
	Engine::win.setIcon("resource/textures/icons/k.ico");

	// Maximizing the window
	Engine::win.maximize();

	// Creating the gpu
	Engine::Render::gpu = std::make_unique<kl::gpu>(Engine::win.getWND());

	// Camera
	Engine::Default::camera = std::make_unique<Engine::Camera>();
	Engine::Render::camera = Engine::Default::camera.get();

	// Resize callback
	Engine::win.resize = Engine::Stage::Resize;

	// Rasters
	Engine::Rasters::solid = Engine::Render::gpu->newRasterState(false, true);
	Engine::Rasters::wire = Engine::Render::gpu->newRasterState(true, false);
	Engine::Rasters::shadow = Engine::Render::gpu->newRasterState(false, true, false);
	Engine::Render::entityRaster = Engine::Rasters::solid;
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
	Engine::Shaders::Vertex::collider = Engine::Render::gpu->newVertexShader(kl::file::read("resource/shaders/collider.hlsl"));
	Engine::Shaders::Pixel::collider = Engine::Render::gpu->newPixelShader(kl::file::read("resource/shaders/collider.hlsl"));
	Engine::Shaders::Vertex::gizmo = Engine::Render::gpu->newVertexShader(kl::file::read("resource/shaders/gizmo.hlsl"));
	Engine::Shaders::Pixel::gizmo = Engine::Render::gpu->newPixelShader(kl::file::read("resource/shaders/gizmo.hlsl"));
	Engine::Render::gpu->bind(defaultLayout);

	// Constant buffers
	Engine::CBuffers::buff16_1 = Engine::Render::gpu->newConstBuffer(16);
	Engine::CBuffers::buff16_2 = Engine::Render::gpu->newConstBuffer(16);
	Engine::CBuffers::buff32_1 = Engine::Render::gpu->newConstBuffer(32);
	Engine::CBuffers::buff32_2 = Engine::Render::gpu->newConstBuffer(32);
	Engine::CBuffers::buff48_1 = Engine::Render::gpu->newConstBuffer(48);
	Engine::CBuffers::buff48_2 = Engine::Render::gpu->newConstBuffer(48);
	Engine::CBuffers::buff64_1 = Engine::Render::gpu->newConstBuffer(64);
	Engine::CBuffers::buff64_2 = Engine::Render::gpu->newConstBuffer(64);
	Engine::CBuffers::buff96_1 = Engine::Render::gpu->newConstBuffer(96);
	Engine::CBuffers::buff96_2 = Engine::Render::gpu->newConstBuffer(96);
	Engine::CBuffers::buff192_1 = Engine::Render::gpu->newConstBuffer(192);
	Engine::CBuffers::buff192_2 = Engine::Render::gpu->newConstBuffer(192);

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
	Engine::Outline::screenM = Engine::Render::gpu->newVertBuffer({
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});
	ID3D11Texture2D* outlineTex = Engine::Render::gpu->newTexture(&pickTexDes);
	Engine::Outline::targetV = Engine::Render::gpu->newTargetView(outlineTex);
	Engine::Outline::shaderV = Engine::Render::gpu->newShaderView(outlineTex);
	Engine::Render::gpu->destroy(outlineTex);

	// Sun
	Engine::Light::sun.genBuff(Engine::Render::gpu.get(), 4096);
	Engine::Light::sun.direction = kl::float3(0.575f, -0.75f, 2.0f);

	// Default meshes
	Engine::Default::cube = new Engine::Mesh("cube", kl::file::parseObj("resource/meshes/default/cube.obj"));
	Engine::Default::sphere = new Engine::Mesh("sphere", kl::file::parseObj("resource/meshes/default/sphere.obj"));
	Engine::Default::capsule = new Engine::Mesh("capsule", kl::file::parseObj("resource/meshes/default/capsule.obj"));
	Engine::Default::pyramid = new Engine::Mesh("pyramid", kl::file::parseObj("resource/meshes/default/pyramid.obj"));
	Engine::Default::monke = new Engine::Mesh("monke", kl::file::parseObj("resource/meshes/default/monke.obj"));

	// Gizmos
	Engine::Gizmo::scaleM = Engine::Render::gpu->newVertBuffer("resource/meshes/gizmo/scale.obj");
	Engine::Gizmo::moveM = Engine::Render::gpu->newVertBuffer("resource/meshes/gizmo/move.obj");
	Engine::Gizmo::rotateM = Engine::Render::gpu->newVertBuffer("resource/meshes/gizmo/rotate.obj");

	// Default textures
	Engine::Default::texture = new Engine::Texture("default", kl::image(kl::int2(1), kl::colors::magenta));

	// GUI icons
	Engine::GUI::folderIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/explorer/folder.png").flipV()));
	Engine::GUI::folderEIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/explorer/folder_empty.png").flipV()));
	Engine::GUI::fileIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/explorer/file.png").flipV()));
	Engine::GUI::imageIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/explorer/image.png").flipV()));
	Engine::GUI::objectIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/explorer/object.png").flipV()));
	Engine::GUI::codeIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/explorer/code.png").flipV()));
	Engine::GUI::scriptIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/explorer/script.png").flipV()));
	Engine::GUI::solidRaIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/viewport/solid.png").flipV()));
	Engine::GUI::solidRaGIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/viewport/solid_gray.png").flipV()));
	Engine::GUI::wireRaIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/viewport/wire.png").flipV()));
	Engine::GUI::wireRaGIcon = Engine::Render::gpu->newShaderView(
		Engine::Render::gpu->newTexture(kl::image("resource/textures/viewport/wire_gray.png").flipV()));

	// Console fix
	kl::console::hide();
}
