#include "Input/Input.h"
#include "Types/Camera.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void MovementSetup() {
	Engine::win.keys.w.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveForward(Engine::Time::delta);
		}
	};
	Engine::win.keys.s.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveBack(Engine::Time::delta);
		}
	};
	Engine::win.keys.d.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveRight(Engine::Time::delta);
		}
	};
	Engine::win.keys.a.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveLeft(Engine::Time::delta);
		}
	};
	Engine::win.keys.e.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveUp(Engine::Time::delta);
		}
	};
	Engine::win.keys.q.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveDown(Engine::Time::delta);
		}
	};
	Engine::win.keys.shift.press = [&]() {
		Engine::Selected::camera->speed = 5.0f;
	};
	Engine::win.keys.shift.release = [&]() {
		Engine::Selected::camera->speed = 2.0f;
	};
}

bool firstClick = true;
bool camMoving = false;
void RotationSetup() {
	Engine::win.mouse.rmb.press = [&]() {
		if (Engine::GUI::viewportFocus) {
			Engine::win.mouse.hide();
			camMoving = true;
		}
	};
	Engine::win.mouse.rmb.down = [&]() {
		if (camMoving) {
			// Window center
			const kl::int2 frameCenter = Engine::win.getCenter();

			// First click jump fix
			if (firstClick) {
				Engine::win.mouse.position = frameCenter;
				firstClick = false;
			}

			// Camera rotation
			Engine::Selected::camera->rotate(Engine::win.mouse.position, frameCenter);
			Engine::win.mouse.move(frameCenter);
		}
	};
	Engine::win.mouse.rmb.release = [&]() {
		Engine::win.mouse.show();
		firstClick = true;
		camMoving = false;
	};
}

void Engine::Input::Camera() {
	// Movement
	MovementSetup();

	// Camera rotation
	RotationSetup();
}
