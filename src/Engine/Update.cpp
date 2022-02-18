#include "Engine/Engine.h"


void Update() {
	// Time
	deltaT = timer.interval();
	elapsedT = timer.elapsed();

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

	// Drawing entities
	Draw();

	// Highlighting
	Highlight();

	// Drawing the gui
	kl::igui::draw(GUI);

	// Swapping the frame buffers
	gpu->swap(true);
}
