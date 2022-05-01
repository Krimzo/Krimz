#include "Render/Render.h"
#include "View/Background.h"
#include "Input/Picking.h"


void Engine::Render::Skybox() {
	// Binding internal
	Engine::Render::gpu->bindTargets({ Engine::Render::targetV });

	// Set raster
	Engine::Render::gpu->bind(Engine::Rasters::solid);

	// Disable depth
	Engine::Render::gpu->bind(Engine::DepthStencil::disabled);

	// Render box
	Engine::Background::skybox->render(Engine::Render::camera.matrix());
}
