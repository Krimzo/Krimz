#include "Engine/Engine.h"


void Shadows() {
	// Binding the shadow render target
	sun.shadowMap->bindTarget();

	// Setting the viewport
	sun.shadowMap->fixViewport();

	// Clearing the depth
	sun.shadowMap->clear();

	// Front culling
	shadow_ra->bind();

	// Binding the shadow shaders
	shadow_sh->bind();

	// Getting the sun vp matrix
	const kl::mat4 vpSun = sun.matrix(camera);

	// Rendering entity shadows
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->shadows) {
			// Getting the full wvp matrix
			kl::mat4 wvp = vpSun * entities[i]->matrix();
			shadow_sh->setVertData(&wvp);
	
			// Rendering the entity
			entities[i]->render(false);
		}
	}

	// Binding the default states
	gpu->bindInternal();
	gpu->setDSState(kl::dbuffer::Default);

	// Viewport reset
	gpu->setViewport(kl::int2(0, 0), win.getSize());

	// Raster reset
	solid_ra->bind();
}
