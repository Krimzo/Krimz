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
		gpu->setDSState(kl::dbuffer::Disabled);
		skybox->render(camera.matrix());
		gpu->setDSState(kl::dbuffer::Default);
	}

	// Drawing entities
	Draw();

	// Selected postprocess
	if (selected) {
		// Entity outline
		Outline();

		// Drawing the gizmos
		Gizmo();
	}

	// Drawing the gui
	kl::igui::draw(GUI);

	// Swapping the frame buffers
	gpu->swap(true);
}
