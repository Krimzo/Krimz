#include "Engine/Input.h"


bool num1WasDown = false;
bool num2WasDown = false;
bool num3WasDown = false;
void GizmoInput() {
	// Key 1
	if (win.keys.num1) {
		if (!num1WasDown) {
			if (chosenGizmo == GIZMO_SCALE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_SCALE;
			}
		}
		num1WasDown = true;
	}
	else {
		num1WasDown = false;
	}

	// Key 2
	if (win.keys.num2) {
		if (!num2WasDown) {
			if (chosenGizmo == GIZMO_MOVE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_MOVE;
			}
		}
		num2WasDown = true;
	}
	else {
		num2WasDown = false;
	}

	// Key 3
	if (win.keys.num3) {
		if (!num3WasDown) {
			if (chosenGizmo == GIZMO_ROTATE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_ROTATE;
			}
		}
		num3WasDown = true;
	}
	else {
		num3WasDown = false;
	}
}
