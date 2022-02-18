#include "Engine/Engine.h"


void Highlight() {
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
}
