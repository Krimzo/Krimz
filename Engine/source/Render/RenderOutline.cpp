#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"


void Engine::Render::Outline() {
	Engine::gpu->bindTargets({ Engine::Outline::targetView });
	Engine::gpu->clear(Engine::Outline::targetView, kl::float4(-1.0f));
	Engine::gpu->bind(Engine::Selected::raster);
	Engine::gpu->bind(Engine::DepthStencil::disabled);
	Engine::gpu->bind(Engine::Shaders::index);

	const kl::mat4 wvp = Engine::Selected::camera->matrix() * Engine::Selected::entity->matrix();
	Engine::gpu->autoVertexCBuffer(wvp);
	Engine::Selected::entity->render(false);

	Engine::gpu->bindTargets({ Engine::Render::targetView });
	Engine::gpu->bind(Engine::Rasters::solid);
	Engine::gpu->bind(Engine::Shaders::outline);

	const kl::float4 higCol = Engine::Outline::color;
	Engine::gpu->autoPixelCBuffer(higCol);
	Engine::gpu->bindPixelShaderView(Engine::Outline::shaderView, 0);
	Engine::gpu->bind(Engine::DepthStencil::mask);
	Engine::gpu->draw(Engine::Meshes::Util::screen->buffer);
	Engine::gpu->bind(Engine::DepthStencil::depth);
}
