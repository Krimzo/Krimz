#include "Engine/Engine.h"


void Update() {
	// Time
	deltaT = timer.interval();
	elapsedT = timer.elapsed();

	// Input call
	Input();

	// Rendering the shadows
	Shadows();

	// Clearing the buffers
	gpu->clear(background);

	// Rendering skybox
	if (skybox) {
		gpu->setDepthTest(false);
		skybox->render(camera.matrix());
		gpu->setDepthTest(true);
	}

	// Drawing entities
	Draw();

	// Selected postprocess
	if (selected) {
		// Wireframe highlight
		Highlight();

		// Drawing the gizmos
		Gizmo();
	}

	// Drawing the gui
	kl::igui::draw(GUI);

	// Swapping the frame buffers
	gpu->swap(true);
}
