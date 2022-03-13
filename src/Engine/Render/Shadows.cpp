#include "Engine/Engine.h"


void Shadows() {
	// Binding the shadow render target
	gpu->bindTargets({}, sun.shadowMapDV);

	// Setting the viewport
	gpu->viewport(kl::int2(0), kl::int2(4096));

	// Setting default depth state
	gpu->bind(depth_ds);

	// Front culling
	gpu->bind(shadow_ra);

	// Clearing the shadow depth
	gpu->clear(sun.shadowMapDV);

	// Binding the shadow shaders
	gpu->bind(shadow_vtx);
	gpu->bind(shadow_pxl);
	gpu->bindVertCBuff(shadow_vtx_cb, 0);

	// Getting the sun vp matrix
	const kl::mat4 vpSun = sun.matrix(camera);

	// Rendering entity shadows
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->shadows) {
			// Getting the full wvp matrix
			kl::mat4 wvp = vpSun * entities[i]->matrix();
			gpu->setBuffData(shadow_vtx_cb, &wvp);

			// Rendering the entity
			entities[i]->render(gpu, false);
		}
	}

	// Binding the default states
	gpu->bindInternal();

	// Raster reset
	gpu->bind(solid_ra);
}
