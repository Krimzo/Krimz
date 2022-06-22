#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"


void Engine::Render::Skybox() {
	Engine::gpu->bindTargets({ Engine::Render::targetView });
	Engine::gpu->bind(Engine::Rasters::skybox);
	Engine::gpu->bind(Engine::DepthStencil::disabled);
	Engine::Selected::camera->skybox->render(Engine::Selected::camera->matrix());
}
