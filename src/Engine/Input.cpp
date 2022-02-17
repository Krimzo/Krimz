#include "Engine/Engine.h"


bool camMoving = false;

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
	if (win.keys.shift && win.mouse.lmb) {
		camMoving = true;
		win.mouse.hide();

		// Fixing the camera jump on the first click
		win.mouse.position = win.getCenter();
	}
	if (win.keys.shift && win.mouse.rmb) {
		camMoving = false;
		win.mouse.show();
	}
	if (camMoving) {
		const kl::ivec2 frameCenter = win.getCenter();
		camera.rotate(win.mouse.position, frameCenter);
		win.mouse.move(frameCenter);
	}

	// Picking
	if (!win.keys.shift && win.mouse.rmb) {
		const int ind = gpu->getPickingIndex(win.mouse.position);
		if (ind >= 0) {
			selected = entities[ind];
		}
		else {
			selected = nullptr;
		}
	}
}
