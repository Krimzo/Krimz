#include "Engine/Input/Input.h"


void Engine::Input::Gizmo() {
	Engine::Window::win.keys.num1.press = [&]() {
		if (Engine::Gizmo::selected == Engine::Gizmo::Type::SCALE) {
			Engine::Gizmo::selected = Engine::Gizmo::Type::NONE;
		}
		else {
			Engine::Gizmo::selected = Engine::Gizmo::Type::SCALE;
		}
	};
	Engine::Window::win.keys.num2.press = [&]() {
		if (Engine::Gizmo::selected == Engine::Gizmo::Type::MOVE) {
			Engine::Gizmo::selected = Engine::Gizmo::Type::NONE;
		}
		else {
			Engine::Gizmo::selected = Engine::Gizmo::Type::MOVE;
		}
	};
	Engine::Window::win.keys.num3.press = [&]() {
		if (Engine::Gizmo::selected == Engine::Gizmo::Type::ROTATE) {
			Engine::Gizmo::selected = Engine::Gizmo::Type::NONE;
		}
		else {
			Engine::Gizmo::selected = Engine::Gizmo::Type::ROTATE;
		}
	};
}
