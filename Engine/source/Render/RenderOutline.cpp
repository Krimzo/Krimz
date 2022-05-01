#include "Render/Render.h"
#include "Input/Picking.h"


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
	Engine::Render::gpu->bindVertCBuff(Engine::CBuffers::buff64_1, 0);

	// Setting the index vertex data
	kl::mat4 wvp = Engine::Render::camera.matrix() * Engine::Picking::selected->matrix();
	Engine::Render::gpu->setBuffData(Engine::CBuffers::buff64_1, &wvp);

	// Drawing the selected
	Engine::Picking::selected->render(Engine::Render::gpu, false);

	// Render target reset
	Engine::Render::gpu->bindTargets({ Engine::Render::targetV });

	// Raster bind
	Engine::Render::gpu->bind(Engine::Rasters::solid);

	// Binding outline shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::outline);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::outline);
	Engine::Render::gpu->bindPixlCBuff(Engine::CBuffers::buff16_1, 0);

	// Setting the outline pixel data
	kl::float4 higCol = Engine::Outline::color;
	Engine::Render::gpu->setBuffData(Engine::CBuffers::buff16_1, &higCol);

	// Binding the outline index tex
	Engine::Render::gpu->bindPixlTex(Engine::Outline::shaderV, 0);

	// Drawing the outline
	Engine::Render::gpu->bind(Engine::DepthStencil::mask);
	Engine::Render::gpu->draw(Engine::Outline::screenM);
	Engine::Render::gpu->bind(Engine::DepthStencil::depth);
}
