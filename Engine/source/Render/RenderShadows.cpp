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
	Engine::Render::gpu->bindVertCBuff(Engine::CBuffers::Vertex::shadow, 0);

	// Getting the sun vp matrix
	const kl::mat4 vpSun = Engine::Light::sun.matrix(Engine::Render::camera);

	// Rendering entity shadows
	for (int i = 0; i < Engine::entities.size(); i++) {
		if (Engine::entities[i]->shadows) {
			// Getting the full wvp matrix
			kl::mat4 wvp = vpSun * Engine::entities[i]->matrix();
			Engine::Render::gpu->setBuffData(Engine::CBuffers::Vertex::shadow, &wvp);

			// Rendering the entity
			Engine::entities[i]->render(Engine::Render::gpu, false);
		}
	}

	// Binding the default states
	Engine::Render::gpu->bindInternal();

	// Raster reset
	Engine::Render::gpu->bind(Engine::Rasters::solid);
}
