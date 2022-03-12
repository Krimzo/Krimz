#include "Engine/Engine.h"


void Draw() {
	// Binding internal + index texture
	gpu->bindInternal({ pickingTargetV });

	// Clearing picking tex
	gpu->clear(pickingTargetV, kl::float4(-1.0f));

	// Binding the editor shaders
	gpu->bind(editor_vtx);
	gpu->bind(editor_pxl);
	gpu->bindVertCBuff(editor_vtx_cb, 0);
	gpu->bindPixlCBuff(editor_pxl_cb, 0);

	// Binding the shadow map
	gpu->bindPixlTex(sun.shadowMapSV, 1);

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
			gpu->setBuffData(editor_vtx_cb, &draw_vert_data);

			// Updating the pixl data
			draw_pixl_data.rghFac.x = entities[i]->roughness;
			draw_pixl_data.objInd.x = float(i);
			gpu->setBuffData(editor_pxl_cb, &draw_pixl_data);

			// Rendering the entity
			if (entities[i] == selected) {
				gpu->bind(write_ds);
				entities[i]->render(gpu, true);
				gpu->bind(depth_ds);
			}
			else {
				entities[i]->render(gpu, true);
			}
		}
	}
}
