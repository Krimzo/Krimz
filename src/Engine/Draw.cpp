#include "Engine/Engine.h"


void Draw() {
	// Setting the camera data
	EDI_VS_CB edi_vert_data = {};
	edi_vert_data.vp = camera.matrix();

	// Setting the lighting data
	EDI_PS_CB edi_pixl_data = {};
	edi_pixl_data.ambient = ambient.getCol();
	edi_pixl_data.dirCol = sun.getCol();
	edi_pixl_data.dirDir = sun.getDir();

	// Rendering entities
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->visible) {
			// Setting the world matrix
			edi_vert_data.w = entities[i]->matrix();

			// Updating the vert data
			editor_sh->setVertData(&edi_vert_data);

			// Setting the obj index
			edi_pixl_data.objIndex.x = *(float*)&i;

			// Updating the pixl data
			editor_sh->setPixlData(&edi_pixl_data);

			// Rendering the entity
			entities[i]->render();
		}
	}
}
