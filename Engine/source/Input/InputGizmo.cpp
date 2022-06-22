#include "Input/Input.h"
#include "Editor/Gizmo.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void Engine::Input::Gizmo() {
	Engine::window.keys.num1.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Engine::Selected::gizmo == Engine::Gizmo::Type::SCALE) {
				Engine::Selected::gizmo = Engine::Gizmo::Type::NONE;
			}
			else {
				Engine::Selected::gizmo = Engine::Gizmo::Type::SCALE;
			}
		}
	};
	Engine::window.keys.num2.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Engine::Selected::gizmo == Engine::Gizmo::Type::MOVE) {
				Engine::Selected::gizmo = Engine::Gizmo::Type::NONE;
			}
			else {
				Engine::Selected::gizmo = Engine::Gizmo::Type::MOVE;
			}
		}
	};
	Engine::window.keys.num3.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Engine::Selected::gizmo == Engine::Gizmo::Type::ROTATE) {
				Engine::Selected::gizmo = Engine::Gizmo::Type::NONE;
			}
			else {
				Engine::Selected::gizmo = Engine::Gizmo::Type::ROTATE;
			}
		}
	};
}
