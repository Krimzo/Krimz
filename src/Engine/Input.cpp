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

	// Gizmo setting
	static bool num1WasDown = false;
	if (win.keys.num1) {
		if (!num1WasDown) {
			if (chosenGizmo == GIZMO_SCALE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_SCALE;
			}
		}
		num1WasDown = true;
	}
	else {
		num1WasDown = false;
	}
	static bool num2WasDown = false;
	if (win.keys.num2) {
		if (!num2WasDown) {
			if (chosenGizmo == GIZMO_MOVE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_MOVE;
			}
		}
		num2WasDown = true;
	}
	else {
		num2WasDown = false;
	}
	static bool num3WasDown = false;
	if (win.keys.num3) {
		if (!num3WasDown) {
			if (chosenGizmo == GIZMO_ROTATE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_ROTATE;
			}
		}
		num3WasDown = true;
	}
	else {
		num3WasDown = false;
	}

	// LMB
	static bool lmbWasDown = false;
	static kl::ivec2 lastLmbPos;
	if (win.mouse.lmb) {
		// Mouse over GUI check
		if (!ImGui::GetIO().WantCaptureMouse) {
			// Entity select
			const int ind = gpu->getIndex(win.mouse.position);
			if (ind >= 0) {
				selected = entities[ind];
			}
			else if (ind >= -2) {
				selected = nullptr;
			}
			else if (chosenGizmo != GIZMO_NONE) {

			}
			lmbWasDown = true;
		}
	}
	else {
		lmbWasDown = false;
	}
}
