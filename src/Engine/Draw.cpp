#include "Engine/Engine.h"


void Draw() {
	// Binding the editor shaders
	editor_sh->bind();

	// Binding the shadow map
	sun.shadowMap->bindTexture(1, 1);

	// Setting the camera data
	DRAW_VS_CB draw_vert_data = {};
	draw_vert_data.vpCam = camera.matrix();
	draw_vert_data.vpSun = sun.matrix(camera);

	// Setting the lighting data
	DRAW_PS_CB draw_pixl_data = {};
	draw_pixl_data.ambCol = ambient.getCol();
	draw_pixl_data.dirCol = sun.getCol();
	draw_pixl_data.dirDir = sun.getDir();
	draw_pixl_data.camPos = camera.position;

	// Rendering entities
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->visible) {
			// Updating the vert data
			draw_vert_data.w = entities[i]->matrix();
			editor_sh->setVertData(&draw_vert_data);

			// Updating the pixl data
			draw_pixl_data.rghFac.x = min(max(entities[i]->roughness, 0.0f), 1.0f);
			draw_pixl_data.objInd.x = float(i);
			editor_sh->setPixlData(&draw_pixl_data);

			// Rendering the entity
			if (entities[i] == selected) {
				gpu->setDSState(kl::dbuffer::Write);
				entities[i]->render(true);
				gpu->setDSState(kl::dbuffer::Default);
			}
			else {
				entities[i]->render(true);
			}
		}
	}
}
