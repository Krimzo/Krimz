#include "Render/Render.h"
#include "Types/Camera.h"
#include "Types/Entity.h"


void Engine::Render::Shadows() {
	Engine::gpu->viewport(kl::int2(0), Engine::Light::sun->size());
	Engine::gpu->bind(Engine::Rasters::solid);
	Engine::gpu->bind(Engine::DepthStencil::depth);
	Engine::gpu->bind(Engine::Shaders::shadow);

	for (int i = 0; i < 4; i++) {
		Engine::gpu->bindTargets({}, Engine::Light::sun->depthView(i));
		Engine::gpu->clear(Engine::Light::sun->depthView(i));

		const kl::mat4 vpSun = Engine::Light::sun->matrix(*Engine::Selected::camera, i);
		for (auto& ent : Engine::entities) {
			if (ent->shadows) {
				const kl::mat4 wvp = vpSun * ent->matrix();
				Engine::gpu->autoVertexCBuffer(wvp);
				ent->render(false);
			}
		}
	}
}
