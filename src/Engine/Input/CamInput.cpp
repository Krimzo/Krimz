#include "Engine/Input.h"

#include "KrimzLib/utility/time.h"


void MovementSetup() {
	win.keys.w.down = [&]() { camera.moveForward(deltaT); };
	win.keys.s.down = [&]() { camera.moveBack(deltaT); };
	win.keys.d.down = [&]() { camera.moveRight(deltaT); };
	win.keys.a.down = [&]() { camera.moveLeft(deltaT); };
	win.keys.space.down = [&]() { camera.moveUp(deltaT); };
	win.keys.c.down = [&]() { camera.moveDown(deltaT); };
	win.keys.shift.press = [&]() { camera.speed = 5.0f; };
	win.keys.shift.release = [&]() { camera.speed = 2.0f; };
}

bool firstClick = true;
void RotationSetup() {
	win.mouse.rmb.press = [&]() {
		win.mouse.hide();
	};
	win.mouse.rmb.down = [&]() {
		// Window center
		const kl::int2 frameCenter = win.getCenter();

		// First click jump fix
		if (firstClick) {
			win.mouse.position = frameCenter;
			firstClick = false;
		}

		// Camera rotation
		camera.rotate(win.mouse.position, frameCenter);
		win.mouse.move(frameCenter);
	};
	win.mouse.rmb.release = [&]() {
		win.mouse.show();
		firstClick = true;
	};
}

void CamInputSetup() {
	// Movement
	MovementSetup();

	// Camera rotation
	RotationSetup();
}
