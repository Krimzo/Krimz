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

	// Rendering entities
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->visible) {
			// Setting the world matrix
			draw_vert_data.w = entities[i]->matrix();

			// Updating the vert data
			editor_sh->setVertData(&draw_vert_data);

			// Setting the obj index
			draw_pixl_data.objInd.x = float(i);

			// Updating the pixl data
			editor_sh->setPixlData(&draw_pixl_data);

			// Rendering the entity
			entities[i]->render(true);
		}
	}
}
