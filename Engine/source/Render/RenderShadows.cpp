#include "Render/Render.h"
#include "Data/Entities.h"


void Engine::Render::Shadows() {
	// Setting the viewport
	Engine::Render::gpu->viewport(kl::int2(0), Engine::Light::sun->getSize());

	// Front culling
	Engine::Render::gpu->bind(Engine::Rasters::shadow);

	// Setting default depth state
	Engine::Render::gpu->bind(Engine::DepthStencil::depth);

	// Binding the shadow shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::shadow);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::shadow);

	// Render
	for (int i = 0; i < 4; i++) {
		// Binding the shadow render target
		Engine::Render::gpu->bindTargets({}, Engine::Light::sun->getDepthV(i));

		// Clearing the shadow depth
		Engine::Render::gpu->clear(Engine::Light::sun->getDepthV(i));

		// Getting the sun vp matrix
		const kl::mat4 vpSun = Engine::Light::sun->matrix(*Engine::Selected::camera, i);

		// Rendering entity shadows
		for (auto& ent : Engine::entities) {
			if (ent->shadows) {
				// Getting the full wvp matrix
				kl::mat4 wvp = vpSun * ent->matrix();
				Engine::Render::gpu->autoSetVertBuff(wvp);

				// Rendering the entity
				ent->render(false);
			}
		}
	}
}
