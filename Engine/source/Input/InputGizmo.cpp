#include "Input/Input.h"
#include "Editor/Gizmo.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void Krimz::Input::Gizmo() {
	Krimz::window.keys.num1.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Krimz::Selected::gizmo == Krimz::Gizmo::Type::SCALE) {
				Krimz::Selected::gizmo = Krimz::Gizmo::Type::NONE;
			}
			else {
				Krimz::Selected::gizmo = Krimz::Gizmo::Type::SCALE;
			}
		}
	};
	Krimz::window.keys.num2.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Krimz::Selected::gizmo == Krimz::Gizmo::Type::MOVE) {
				Krimz::Selected::gizmo = Krimz::Gizmo::Type::NONE;
			}
			else {
				Krimz::Selected::gizmo = Krimz::Gizmo::Type::MOVE;
			}
		}
	};
	Krimz::window.keys.num3.press = [&]() {
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			if (Krimz::Selected::gizmo == Krimz::Gizmo::Type::ROTATE) {
				Krimz::Selected::gizmo = Krimz::Gizmo::Type::NONE;
			}
			else {
				Krimz::Selected::gizmo = Krimz::Gizmo::Type::ROTATE;
			}
		}
	};
}
