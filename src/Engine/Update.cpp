#include "Engine/Engine.h"


void Update() {
	// Input call
	Input();

	// Clearing the frame buffer
	gpu->clear(background);

	// Rendering skybox
	if (skybox) {
		gpu->setDepthTest(false);
		skybox->render(camera.matrix());
		gpu->setDepthTest(true);
	}

	// Setting the camera data
	DEF_VS_CB def_vert_data = {};
	def_vert_data.vp = camera.matrix();

	// Setting the lighting data
	DEF_PS_CB def_pixl_data = {};
	def_pixl_data.ambient = ambient.getCol();
	def_pixl_data.dirCol = sun.getCol();
	def_pixl_data.dirDir = sun.getDir();
	default_sh->setPixlData(&def_pixl_data);

	// Rendering objects
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->visible) {
			// Setting the world matrix
			def_vert_data.w = entities[i]->matrix();

			// Updating the cb data
			default_sh->setVertData(&def_vert_data);

			// Rendering the object
			entities[i]->render();
		}
	}

	// Highlighting
	if (selected) {
		// Setting the highlight vertex data
		kl::mat4 wvp = camera.matrix() * selected->matrix();
		highlight_sh->setVertData(&wvp);

		// Setting the highlight pixel data
		kl::vec4 hig_col = highlight;
		highlight_sh->setPixlData(&hig_col);

		// Rendering
		gpu->setDepthTest(false);
		wire_ra->bind();
		selected->render();
		solid_ra->bind();
		gpu->setDepthTest(true);
	}

	// Swapping the frame buffers
	gpu->swap(true);
}
