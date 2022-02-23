#include "Engine/Engine.h"


void Highlight() {
	// Setting the highlight wvp matrix
	kl::mat4 wvp = camera.matrix() * selected->matrix();
	highlight_sh->setVertData(&wvp);

	// Setting the highlight pixel color
	kl::vec4 hig = highlight;
	highlight_sh->setPixlData(&hig);

	// Rendering
	gpu->setDepthTest(false);
	wire_ra->bind();
	selected->render(false);
	solid_ra->bind();
	gpu->setDepthTest(true);
}
