#include "Engine/Engine.h"


void Input() {
	// Exit
	if (win.keys.esc) win.stop();

	// Movement
	if (win.keys.w) camera.moveForward(deltaT);
	if (win.keys.s) camera.moveBack(deltaT);
	if (win.keys.d) camera.moveRight(deltaT);
	if (win.keys.a) camera.moveLeft(deltaT);
	if (win.keys.space) camera.moveUp(deltaT);
	if (win.keys.c) camera.moveDown(deltaT);

	// Speed
	if (win.keys.shift) {
		camera.speed = 5;
	}
	else {
		camera.speed = 2;
	}

	// Camera rotation
	static bool camMoving = false;
	if (win.mouse.rmb) {
		// Getting the frame center
		const kl::ivec2 frameCenter = win.getCenter();

		// Fixing the camera jump on the first click
		if (!camMoving) {
			win.mouse.position = frameCenter;
		}

		// Saving info
		win.mouse.hide();
		camMoving = true;

		// Updating the cam
		camera.rotate(win.mouse.position, frameCenter);
		win.mouse.move(frameCenter);
	}
	else {
		// Saving info
		win.mouse.show();
		camMoving = false;
	}

	// Picking
	if (win.mouse.lmb) {
		// Mouse over GUI check
		if (!ImGui::GetIO().WantCaptureMouse) {
			const int ind = gpu->getIndex(win.mouse.position);
			if (ind >= 0) {
				selected = entities[ind];
			}
			else {
				selected = nullptr;
			}
		}
	}
}
