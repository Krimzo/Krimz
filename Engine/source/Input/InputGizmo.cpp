#include "Input/Input.h"
#include "Editor/Gizmo.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void Engine::Input::Gizmo() {
	Engine::win.keys.num1.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Engine::Gizmo::selected == Engine::Gizmo::Type::SCALE) {
				Engine::Gizmo::selected = Engine::Gizmo::Type::NONE;
			}
			else {
				Engine::Gizmo::selected = Engine::Gizmo::Type::SCALE;
			}
		}
	};
	Engine::win.keys.num2.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Engine::Gizmo::selected == Engine::Gizmo::Type::MOVE) {
				Engine::Gizmo::selected = Engine::Gizmo::Type::NONE;
			}
			else {
				Engine::Gizmo::selected = Engine::Gizmo::Type::MOVE;
			}
		}
	};
	Engine::win.keys.num3.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Engine::Gizmo::selected == Engine::Gizmo::Type::ROTATE) {
				Engine::Gizmo::selected = Engine::Gizmo::Type::NONE;
			}
			else {
				Engine::Gizmo::selected = Engine::Gizmo::Type::ROTATE;
			}
		}
	};
}
