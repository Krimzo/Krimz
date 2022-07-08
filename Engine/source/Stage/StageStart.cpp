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
	Engine::window.icon("resource/textures/icons/k.ico");
	Engine::window.maximize();

	Engine::gpu = kl::make<kl::gpu>(Engine::window);

	Engine::Default::camera = kl::make<Engine::Camera>();
	Engine::Selected::camera = Engine::Default::camera;

	Engine::window.resize = Engine::Stage::Resize;

	Engine::Rasters::solid = Engine::gpu->newRasterState(false, true);
	Engine::Rasters::wire = Engine::gpu->newRasterState(true, false);
	Engine::Rasters::skybox = Engine::gpu->newRasterState(false, false);
	Engine::Selected::raster = Engine::Rasters::solid;
	Engine::gpu->bind(Engine::Selected::raster);

	Engine::DepthStencil::disabled = Engine::gpu->newDepthState(false, false, false);
	Engine::DepthStencil::depth = Engine::gpu->newDepthState(true, false, false);
	Engine::DepthStencil::write = Engine::gpu->newDepthState(true, true, false);
	Engine::DepthStencil::mask = Engine::gpu->newDepthState(false, true, true);

	Engine::Shaders::editor = Engine::gpu->newShaders(kl::file::readString("source/Shaders/Editor.hlsl"));
	Engine::Shaders::shadow = Engine::gpu->newShaders(kl::file::readString("source/Shaders/Shadows.hlsl"));
	Engine::Shaders::index = Engine::gpu->newShaders(kl::file::readString("source/Shaders/Index.hlsl"));
	Engine::Shaders::outline = Engine::gpu->newShaders(kl::file::readString("source/Shaders/Outline.hlsl"));
	Engine::Shaders::collider = Engine::gpu->newShaders(kl::file::readString("source/Shaders/Collider.hlsl"));
	Engine::Shaders::gizmo = Engine::gpu->newShaders(kl::file::readString("source/Shaders/Gizmo.hlsl"));
	Engine::Skybox::shaders = Engine::gpu->newShaders(kl::file::readString("source/Shaders/Skybox.hlsl"));

	Engine::gpu->bind(Engine::gpu->newSamplerState(true, true), 0);
	kl::dx::state::desc::sampler shadowSamp = {};
	shadowSamp.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.BorderColor[0] = 1.0f;
	shadowSamp.BorderColor[1] = 1.0f;
	shadowSamp.BorderColor[2] = 1.0f;
	shadowSamp.BorderColor[3] = 1.0f;
	shadowSamp.ComparisonFunc = D3D11_COMPARISON_LESS;
	Engine::gpu->bind(Engine::gpu->newSamplerState(&shadowSamp), 1);

	kl::dx::desc::texture pickTexDes = {};
	pickTexDes.Width = Engine::window.size().x;
	pickTexDes.Height = Engine::window.size().y;
	pickTexDes.MipLevels = 1;
	pickTexDes.ArraySize = 1;
	pickTexDes.Format = DXGI_FORMAT_R32_FLOAT;
	pickTexDes.SampleDesc.Count = 1;
	pickTexDes.Usage = D3D11_USAGE_DEFAULT;
	pickTexDes.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Engine::Picking::texture = Engine::gpu->newTexture(&pickTexDes);
	Engine::Picking::textureStaging = Engine::gpu->newTextureST(Engine::Picking::texture, 1);
	Engine::Picking::targetView = Engine::gpu->newTargetView(Engine::Picking::texture);

	kl::dx::texture outlineTex = Engine::gpu->newTexture(&pickTexDes);
	Engine::Outline::targetView = Engine::gpu->newTargetView(outlineTex);
	Engine::Outline::shaderView = Engine::gpu->newShaderView(outlineTex);
	Engine::gpu->destroy(outlineTex);

	Engine::Light::sun = kl::make<Engine::Light::Direct>(3096);
	Engine::Light::sun->direction({ 0.575f, -0.75f, 2.0f });

	Engine::Meshes::Default::cube = kl::make<Engine::Mesh>("cube", kl::file::parseMesh("resource/meshes/default/cube.obj"));
	Engine::Meshes::Default::sphere = kl::make<Engine::Mesh>("sphere", kl::file::parseMesh("resource/meshes/default/sphere.obj"));
	Engine::Meshes::Default::capsule = kl::make<Engine::Mesh>("capsule", kl::file::parseMesh("resource/meshes/default/capsule.obj"));
	Engine::Meshes::Default::pyramid = kl::make<Engine::Mesh>("pyramid", kl::file::parseMesh("resource/meshes/default/pyramid.obj"));
	Engine::Meshes::Default::monke = kl::make<Engine::Mesh>("monke", kl::file::parseMesh("resource/meshes/default/monke.obj"));

	Engine::Meshes::Util::screen = kl::make<Engine::Mesh>("screen", std::vector<kl::vertex>{
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
			kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
	});

	Engine::Meshes::Gizmo::scale = Engine::gpu->newVertexBuffer("resource/meshes/gizmo/scale.obj");
	Engine::Meshes::Gizmo::move = Engine::gpu->newVertexBuffer("resource/meshes/gizmo/move.obj");
	Engine::Meshes::Gizmo::rotate = Engine::gpu->newVertexBuffer("resource/meshes/gizmo/rotate.obj");

	Engine::Textures::Default::colorMap = kl::make<Engine::Texture>("Default", kl::image(1, kl::colors::gray));
	Engine::Textures::Default::nullMap = kl::make<Engine::Texture>("Null", kl::image(1, kl::colors::black));

	Engine::GUI::folderIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/explorer/folder.png").flipV()));
	Engine::GUI::folderEIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/explorer/folder_empty.png").flipV()));
	Engine::GUI::fileIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/explorer/file.png").flipV()));
	Engine::GUI::imageIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/explorer/image.png").flipV()));
	Engine::GUI::objectIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/explorer/object.png").flipV()));
	Engine::GUI::codeIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/explorer/code.png").flipV()));
	Engine::GUI::scriptIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/explorer/script.png").flipV()));
	Engine::GUI::solidRaIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/viewport/solid.png").flipV()));
	Engine::GUI::solidRaGIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/viewport/solid_gray.png").flipV()));
	Engine::GUI::wireRaIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/viewport/wire.png").flipV()));
	Engine::GUI::wireRaGIcon = Engine::gpu->newShaderView(
		Engine::gpu->newTexture(kl::image("resource/textures/viewport/wire_gray.png").flipV()));

	kl::console::hide();
}
