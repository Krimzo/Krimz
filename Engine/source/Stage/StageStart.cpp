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


void Krimz::Stage::Start() {
	Krimz::Default::camera = kl::make<Krimz::Camera>();
	Krimz::Selected::camera = Krimz::Default::camera;

	Krimz::window.resize = Krimz::Stage::Resize;

	Krimz::Rasters::solid = Krimz::gpu->newRasterState(false, true);
	Krimz::Rasters::wire = Krimz::gpu->newRasterState(true, false);
	Krimz::Rasters::skybox = Krimz::gpu->newRasterState(false, false);
	Krimz::Selected::raster = Krimz::Rasters::solid;
	Krimz::gpu->bind(Krimz::Selected::raster);

	Krimz::DepthStencil::disabled = Krimz::gpu->newDepthState(false, false, false);
	Krimz::DepthStencil::depth = Krimz::gpu->newDepthState(true, false, false);
	Krimz::DepthStencil::write = Krimz::gpu->newDepthState(true, true, false);
	Krimz::DepthStencil::mask = Krimz::gpu->newDepthState(false, true, true);

	Krimz::Shaders::editor = Krimz::gpu->newShaders(kl::file::readString("source/Shaders/Editor.hlsl"));
	Krimz::Shaders::shadow = Krimz::gpu->newShaders(kl::file::readString("source/Shaders/Shadows.hlsl"));
	Krimz::Shaders::index = Krimz::gpu->newShaders(kl::file::readString("source/Shaders/Index.hlsl"));
	Krimz::Shaders::outline = Krimz::gpu->newShaders(kl::file::readString("source/Shaders/Outline.hlsl"));
	Krimz::Shaders::collider = Krimz::gpu->newShaders(kl::file::readString("source/Shaders/Collider.hlsl"));
	Krimz::Shaders::gizmo = Krimz::gpu->newShaders(kl::file::readString("source/Shaders/Gizmo.hlsl"));
	Krimz::Skybox::shaders = Krimz::gpu->newShaders(kl::file::readString("source/Shaders/Skybox.hlsl"));

	Krimz::gpu->bind(Krimz::gpu->newSamplerState(true, true), 0);
	kl::dx::state::desc::sampler shadowSamp = {};
	shadowSamp.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamp.BorderColor[0] = 1.0f;
	shadowSamp.BorderColor[1] = 1.0f;
	shadowSamp.BorderColor[2] = 1.0f;
	shadowSamp.BorderColor[3] = 1.0f;
	shadowSamp.ComparisonFunc = D3D11_COMPARISON_LESS;
	Krimz::gpu->bind(Krimz::gpu->newSamplerState(&shadowSamp), 1);

	kl::dx::desc::texture pickTexDes = {};
	pickTexDes.Width = Krimz::window.size().x;
	pickTexDes.Height = Krimz::window.size().y;
	pickTexDes.MipLevels = 1;
	pickTexDes.ArraySize = 1;
	pickTexDes.Format = DXGI_FORMAT_R32_FLOAT;
	pickTexDes.SampleDesc.Count = 1;
	pickTexDes.Usage = D3D11_USAGE_DEFAULT;
	pickTexDes.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Krimz::Picking::texture = Krimz::gpu->newTexture(&pickTexDes);
	Krimz::Picking::textureStaging = Krimz::gpu->newTextureST(Krimz::Picking::texture, 1);
	Krimz::Picking::targetView = Krimz::gpu->newTargetView(Krimz::Picking::texture);

	kl::dx::texture outlineTex = Krimz::gpu->newTexture(&pickTexDes);
	Krimz::Outline::targetView = Krimz::gpu->newTargetView(outlineTex);
	Krimz::Outline::shaderView = Krimz::gpu->newShaderView(outlineTex);
	Krimz::gpu->destroy(outlineTex);

	Krimz::Light::sun = kl::make<Krimz::Light::Direct>(3096);
	Krimz::Light::sun->direction({ 0.575f, -0.75f, 2.0f });

	Krimz::Meshes::Default::cube = kl::make<Krimz::Mesh>("cube", kl::file::parseMesh("resource/meshes/default/cube.obj"));
	Krimz::Meshes::Default::sphere = kl::make<Krimz::Mesh>("sphere", kl::file::parseMesh("resource/meshes/default/sphere.obj"));
	Krimz::Meshes::Default::capsule = kl::make<Krimz::Mesh>("capsule", kl::file::parseMesh("resource/meshes/default/capsule.obj"));
	Krimz::Meshes::Default::pyramid = kl::make<Krimz::Mesh>("pyramid", kl::file::parseMesh("resource/meshes/default/pyramid.obj"));
	Krimz::Meshes::Default::monke = kl::make<Krimz::Mesh>("monke", kl::file::parseMesh("resource/meshes/default/monke.obj"));

	Krimz::Meshes::Util::screen = kl::make<Krimz::Mesh>("screen", std::vector<kl::vertex>{
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
			kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
	});

	Krimz::Meshes::Gizmo::scale = Krimz::gpu->newVertexBuffer("resource/meshes/gizmo/scale.obj");
	Krimz::Meshes::Gizmo::move = Krimz::gpu->newVertexBuffer("resource/meshes/gizmo/move.obj");
	Krimz::Meshes::Gizmo::rotate = Krimz::gpu->newVertexBuffer("resource/meshes/gizmo/rotate.obj");

	Krimz::Textures::Default::colorMap = kl::make<Krimz::Texture>("Default", kl::image(1, kl::colors::gray));
	Krimz::Textures::Default::nullMap = kl::make<Krimz::Texture>("Null", kl::image(1, kl::colors::black));

	Krimz::GUI::folderIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/explorer/folder.png").flipV()));
	Krimz::GUI::folderEIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/explorer/folder_empty.png").flipV()));
	Krimz::GUI::fileIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/explorer/file.png").flipV()));
	Krimz::GUI::imageIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/explorer/image.png").flipV()));
	Krimz::GUI::objectIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/explorer/object.png").flipV()));
	Krimz::GUI::codeIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/explorer/code.png").flipV()));
	Krimz::GUI::scriptIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/explorer/script.png").flipV()));
	Krimz::GUI::solidRaIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/viewport/solid.png").flipV()));
	Krimz::GUI::solidRaGIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/viewport/solid_gray.png").flipV()));
	Krimz::GUI::wireRaIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/viewport/wire.png").flipV()));
	Krimz::GUI::wireRaGIcon = Krimz::gpu->newShaderView(
		Krimz::gpu->newTexture(kl::image("resource/textures/viewport/wire_gray.png").flipV()));

	kl::console::hide();
}
