#include "Engine/Input.h"


void GizmoHigh() {
	// Color reset
	gizmoColX = kl::color(205, 55, 75);
	gizmoColY = kl::color(115, 175, 40);
	gizmoColZ = kl::color(55, 120, 205);

	// Entity id check
	if (heldIndex >= -2) {
		if (entityIndex == -3) {
			gizmoColX *= 1.45f;
		}
		else if (entityIndex == -4) {
			gizmoColY *= 1.45f;
		}
		else if (entityIndex == -5) {
			gizmoColZ *= 1.45f;
		}
	}
	else {
		if (heldIndex == -3) {
			gizmoColX *= 1.45f;
		}
		else if (heldIndex == -4) {
			gizmoColY *= 1.45f;
		}
		else if (heldIndex == -5) {
			gizmoColZ *= 1.45f;
		}
	}
}
