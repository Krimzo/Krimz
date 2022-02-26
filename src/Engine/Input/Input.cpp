#include "Engine/Input.h"


void Input() {
	// Entity index getter
	entityIndex = gpu->getIndex(win.mouse.position);
	
	// LMB
	LMBInput();

	// Gizmo highlight
	GizmoHigh();

	// Gizmo input
	GizmoInput();

	// Free flight cam
	FreeCam();
}
