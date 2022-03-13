#include "Engine/Engine.h"

#include "Engine/GUI.h"


void Update() {
	// Time
	deltaT = timer.interval();
	elapsedT = timer.elapsed();

	// Clearing the buffers
	gpu->clear(background);

	// Input call
	Input();

	// Rendering shadows
	Shadows();

	//  Gui render
	kl::igui::draw(GUI);

	/* Viewport render */ {
		// Viewport fix
		gpu->viewport(guiViewportPos, guiViewportSize);
		camera.aspect = float(guiViewportSize.x) / guiViewportSize.y;

		// Skybox draw
		if (skybox) {
			gpu->bind(disabled_ds);
			skybox->render(camera.matrix());
			gpu->bind(depth_ds);
		}

		// Entity render
		Draw();

		// Selected postprocess
		if (selected) {
			// Outline draw
			Outline();

			// Gizmo render
			Gizmo();
		}
	}

	// Backbuffer swap
	gpu->swap(true);
}
