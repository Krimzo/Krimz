#include "Input/Input.h"
#include "Types/Camera.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void MovementSetup() {
	Engine::window.keys.w.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveForward(Engine::Time::delta);
		}
	};
	Engine::window.keys.s.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveBack(Engine::Time::delta);
		}
	};
	Engine::window.keys.d.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveRight(Engine::Time::delta);
		}
	};
	Engine::window.keys.a.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveLeft(Engine::Time::delta);
		}
	};
	Engine::window.keys.e.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveUp(Engine::Time::delta);
		}
	};
	Engine::window.keys.q.down = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			Engine::Selected::camera->moveDown(Engine::Time::delta);
		}
	};
	Engine::window.keys.shift.press = [&]() {
		Engine::Selected::camera->speed = 5.0f;
	};
	Engine::window.keys.shift.release = [&]() {
		Engine::Selected::camera->speed = 2.0f;
	};
}

bool firstClick = true;
bool camMoving = false;
void RotationSetup() {
	Engine::window.mouse.rmb.press = [&]() {
		if (Engine::GUI::viewportFocus) {
			Engine::window.mouse.hide();
			camMoving = true;
		}
	};
	Engine::window.mouse.rmb.down = [&]() {
		if (camMoving) {
			const kl::int2 frameCenter = Engine::window.center();

			if (firstClick) {
				Engine::window.mouse.position = frameCenter;
				firstClick = false;
			}

			Engine::Selected::camera->rotate(Engine::window.mouse.position, frameCenter);
			Engine::window.mouse.move(frameCenter);
		}
	};
	Engine::window.mouse.rmb.release = [&]() {
		Engine::window.mouse.show();
		firstClick = true;
		camMoving = false;
	};
}

void Engine::Input::Camera() {
	MovementSetup();
	RotationSetup();
}
