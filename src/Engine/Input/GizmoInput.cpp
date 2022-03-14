#include "Engine/Input.h"


void GizmoInputSetup() {
	win.keys.num1.press = [&]() {
		if (chosenGizmo == GIZMO_SCALE) {
			chosenGizmo = GIZMO_NONE;
		}
		else {
			chosenGizmo = GIZMO_SCALE;
		}
	};
	win.keys.num2.press = [&]() {
		if (chosenGizmo == GIZMO_MOVE) {
			chosenGizmo = GIZMO_NONE;
		}
		else {
			chosenGizmo = GIZMO_MOVE;
		}
	};
	win.keys.num3.press = [&]() {
		if (chosenGizmo == GIZMO_ROTATE) {
			chosenGizmo = GIZMO_NONE;
		}
		else {
			chosenGizmo = GIZMO_ROTATE;
		}
	};
}
