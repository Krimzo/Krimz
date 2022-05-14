#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Scripting/Scripting.h"
#include "Types/Camera.h"
#include "Types/Mesh.h"
#include "Types/Texture.h"
#include "Types/Entity.h"
#include "GUI/GUI.h"


void Engine::Stage::Start() {
	// Setting the icon
	Engine::win.setIcon("resource/textures/icons/k.ico");

	// Maximizing the window
	Engine::win.maximize();

	// Creating the gpu
	Engine::Render::gpu = std::make_unique<kl::gpu>(Engine::win.getWND());

	// Camera
	Engine::Default::camera = std::make_shared<Engine::Camera>();
	Engine::Selected::camera = Engine::Default::camera;

	// Resize callback
	Engine::win.resize = Engine::Stage::Resize;

	// Rasters
	Engine::Rasters::solid = Engine::Render::gpu->newRasterState(false, true);
	Engine::Rasters::wire = Engine::Render::gpu->newRasterState(true, false);
	Engine::Rasters::skybox = Engine::Render::gpu->newRasterState(false, false);
	Engine::Render::entityRaster = Engine::Rasters::solid;
	Engine::Render::gpu->bind(Engine::Render::entityRaster);

	// Depth states
	Engine::DepthStencil::disabled = Engine::Render::gpu->newDepthState(false, false, false);
	Engine::DepthStencil::depth = Engine::Render::gpu->newDepthState(true, false, false);
	Engine::DepthStencil::write = Engine::Render::gpu->newDepthState(true, true, false);
	Engine::DepthStencil::mask = Engine::Render::gpu->newDepthState(false, true, true);

	// Shaders
	Engine::Shaders::editor = Engine::Render::gpu->newShaders(kl::file::read("resource/shaders/editor.hlsl"), kl::file::read("resource/shaders/editor.hlsl"));
	Engine::Shaders::shadow = Engine::Render::gpu->newShaders(kl::file::read("resource/shaders/shadows.hlsl"), kl::file::read("resource/shaders/shadows.hlsl"));
	Engine::Shaders::index = Engine::Render::gpu->newShaders(kl::file::read("resource/shaders/index.hlsl"), kl::file::read("resource/shaders/index.hlsl"));
	Engine::Shaders::outline = Engine::Render::gpu->newShaders(kl::file::read("resource/shaders/outline.hlsl"), kl::file::read("resource/shaders/outline.hlsl"));
	Engine::Shaders::collider = Engine::Render::gpu->newShaders(kl::file::read("resource/shaders/collider.hlsl"), kl::file::read("resource/shaders/collider.hlsl"));
	Engine::Shaders::gizmo = Engine::Render::gpu->newShaders(kl::file::read("resource/shaders/gizmo.hlsl"), kl::file::read("resource/shaders/gizmo.hlsl"));
	Engine::Skybox::shaders = Engine::Render::gpu->newShaders(kl::file::read("resource/shaders/skybox.hlsl"), kl::file::read("resource/shaders/skybox.hlsl"));

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
	Engine::Outline::screenM = Engine::Render::gpu->newVertexBuffer({
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});
	ID3D11Texture2D* outlineTex = Engine::Render::gpu->newTexture(&pickTexDes);
	Engine::Outline::targetV = Engine::Render::gpu->newTargetView(outlineTex);
	Engine::Outline::shaderV = Engine::Render::gpu->newShaderView(outlineTex);
	Engine::Render::gpu->destroy(outlineTex);

	// Sun
	Engine::Light::sun = std::make_shared<Engine::Light::Direct>(kl::int2(3096));
	Engine::Light::sun->setDir(kl::float3(0.575f, -0.75f, 2.0f));

	// Default meshes
	Engine::Default::cube = std::make_shared<Engine::Mesh>("cube", kl::file::parseObj("resource/meshes/default/cube.obj"));
	Engine::Default::sphere = std::make_shared<Engine::Mesh>("sphere", kl::file::parseObj("resource/meshes/default/sphere.obj"));
	Engine::Default::capsule = std::make_shared<Engine::Mesh>("capsule", kl::file::parseObj("resource/meshes/default/capsule.obj"));
	Engine::Default::pyramid = std::make_shared<Engine::Mesh>("pyramid", kl::file::parseObj("resource/meshes/default/pyramid.obj"));
	Engine::Default::monke = std::make_shared<Engine::Mesh>("monke", kl::file::parseObj("resource/meshes/default/monke.obj"));

	// Gizmos
	Engine::Gizmo::scaleM = Engine::Render::gpu->newVertexBuffer("resource/meshes/gizmo/scale.obj");
	Engine::Gizmo::moveM = Engine::Render::gpu->newVertexBuffer("resource/meshes/gizmo/move.obj");
	Engine::Gizmo::rotateM = Engine::Render::gpu->newVertexBuffer("resource/meshes/gizmo/rotate.obj");

	// Default textures
	Engine::Default::colorMap = std::make_shared<Engine::Texture>("Default", kl::image(kl::int2(1), kl::colors::gray));
	Engine::Default::noneMap = std::make_shared<Engine::Texture>("None", kl::image(kl::int2(1), kl::colors::black));

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
