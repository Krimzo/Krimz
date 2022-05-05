#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"


void Engine::Render::Outline() {
	// Binding and clearing the outline buffer
	Engine::Render::gpu->bindTargets({ Engine::Outline::targetV });

	// Clearing indices
	Engine::Render::gpu->clear(Engine::Outline::targetV, kl::float4(-1.0f));

	// Raster bind
	Engine::Render::gpu->bind(Engine::Render::entityRaster);

	// Depth disable
	Engine::Render::gpu->bind(Engine::DepthStencil::disabled);

	// Binding index shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::index);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::index);

	// Setting the index vertex data
	kl::mat4 wvp = Engine::Selected::camera->matrix() * Engine::Selected::entity->matrix();
	Engine::Render::gpu->autoSetVertBuff(wvp);

	// Drawing the selected
	Engine::Selected::entity->render(false);

	// Render target reset
	Engine::Render::gpu->bindTargets({ Engine::Render::targetV });

	// Raster bind
	Engine::Render::gpu->bind(Engine::Rasters::solid);

	// Binding outline shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::outline);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::outline);

	// Setting the outline pixel data
	kl::float4 higCol = Engine::Outline::color;
	Engine::Render::gpu->autoSetPixlBuff(higCol);

	// Binding the outline index tex
	Engine::Render::gpu->bindPixlTex(Engine::Outline::shaderV, 0);

	// Drawing the outline
	Engine::Render::gpu->bind(Engine::DepthStencil::mask);
	Engine::Render::gpu->draw(Engine::Outline::screenM);
	Engine::Render::gpu->bind(Engine::DepthStencil::depth);
}
