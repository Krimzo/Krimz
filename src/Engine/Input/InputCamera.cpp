#include "Engine/Input/Input.h"
#include "Engine/Window/Window.h"
#include "Engine/Render/Render.h"
#include "Engine/Time/Time.h"
#include "Engine/GUI/GUI.h"


void MovementSetup() {
	Engine::Window::win.keys.w.down = [&]() { if (!ImGui::GetIO().WantCaptureKeyboard) Engine::Render::camera.moveForward(Engine::Time::delta); };
	Engine::Window::win.keys.s.down = [&]() { if (!ImGui::GetIO().WantCaptureKeyboard) Engine::Render::camera.moveBack(Engine::Time::delta); };
	Engine::Window::win.keys.d.down = [&]() { if (!ImGui::GetIO().WantCaptureKeyboard) Engine::Render::camera.moveRight(Engine::Time::delta); };
	Engine::Window::win.keys.a.down = [&]() { if (!ImGui::GetIO().WantCaptureKeyboard) Engine::Render::camera.moveLeft(Engine::Time::delta); };
	Engine::Window::win.keys.space.down = [&]() { if (!ImGui::GetIO().WantCaptureKeyboard) Engine::Render::camera.moveUp(Engine::Time::delta); };
	Engine::Window::win.keys.c.down = [&]() { if (!ImGui::GetIO().WantCaptureKeyboard) Engine::Render::camera.moveDown(Engine::Time::delta); };
	Engine::Window::win.keys.shift.press = [&]() { Engine::Render::camera.speed = 5.0f; };
	Engine::Window::win.keys.shift.release = [&]() { Engine::Render::camera.speed = 2.0f; };
}

bool firstClick = true;
bool camMoving = false;
void RotationSetup() {
	Engine::Window::win.mouse.rmb.press = [&]() {
		if (Engine::GUI::viewportFocus) {
			Engine::Window::win.mouse.hide();
			camMoving = true;
		}
	};
	Engine::Window::win.mouse.rmb.down = [&]() {
		if (camMoving) {
			// Window center
			const kl::int2 frameCenter = Engine::Window::win.getCenter();

			// First click jump fix
			if (firstClick) {
				Engine::Window::win.mouse.position = frameCenter;
				firstClick = false;
			}

			// Camera rotation
			Engine::Render::camera.rotate(Engine::Window::win.mouse.position, frameCenter);
			Engine::Window::win.mouse.move(frameCenter);
		}
	};
	Engine::Window::win.mouse.rmb.release = [&]() {
		Engine::Window::win.mouse.show();
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
