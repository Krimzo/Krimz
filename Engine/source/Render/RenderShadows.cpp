#include "Render/Render.h"
#include "Data/Entities.h"
#include "View/Light.h"


void Engine::Render::Shadows() {
	// Binding the shadow render target
	Engine::Render::gpu->bindTargets({}, Engine::Light::sun.shadowMapDV);

	// Setting the viewport
	Engine::Render::gpu->viewport(kl::int2(0), kl::int2(4096));

	// Setting default depth state
	Engine::Render::gpu->bind(Engine::DepthStencil::depth);

	// Front culling
	Engine::Render::gpu->bind(Engine::Rasters::shadow);

	// Clearing the shadow depth
	Engine::Render::gpu->clear(Engine::Light::sun.shadowMapDV);

	// Binding the shadow shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::shadow);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::shadow);
	Engine::Render::gpu->bindVertCBuff(Engine::CBuffers::buff64_1, 0);

	// Getting the sun vp matrix
	const kl::mat4 vpSun = Engine::Light::sun.matrix(Engine::Render::camera);

	// Rendering entity shadows
	for (auto& ent : Engine::entities) {
		if (ent.shadows) {
			// Getting the full wvp matrix
			kl::mat4 wvp = vpSun * ent.matrix();
			Engine::Render::gpu->setBuffData(Engine::CBuffers::buff64_1, &wvp);

			// Rendering the entity
			ent.render(Engine::Render::gpu, false);
		}
	}

	// Binding the default states
	Engine::Render::gpu->bindInternal();
}
