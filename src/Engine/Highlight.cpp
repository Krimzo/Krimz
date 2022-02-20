#include "Engine/Engine.h"


void Highlight() {
	// Setting the highlight vertex data
	HIGH_VS_CB high_vert_data = {};
	high_vert_data.wvp = camera.matrix() * selected->matrix();
	highlight_sh->setVertData(&high_vert_data);

	// Setting the highlight pixel data
	HIGH_PS_CB high_pixl_data = {};
	high_pixl_data.highCol = highlight;
	highlight_sh->setPixlData(&high_pixl_data);

	// Rendering
	gpu->setDepthTest(false);
	wire_ra->bind();
	selected->render();
	solid_ra->bind();
	gpu->setDepthTest(true);
}
