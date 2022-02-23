#include "Engine/Engine.h"


kl::mat4 camMat;
kl::mat4 objMoveMat;

const kl::vec4 xCol = kl::color(205,  55,  75);
const kl::vec4 yCol = kl::color(115, 175,  40);
const kl::vec4 zCol = kl::color( 55, 120, 205);

void DrawGizmo(kl::mesh* toDraw, const kl::vec3& rot, const kl::vec4& col, int index) {
	// Setting the wvp matrix
	kl::mat4 wvp = camMat * (objMoveMat * kl::mat4::rotate(rot));
	gizmo_sh->setVertData(&wvp);

	// Setting the pixl data
	GIZM_PS_CB gizm_pixl_data = {};
	gizm_pixl_data.objCol = col;
	gizm_pixl_data.objInd = float(index);
	gizmo_sh->setPixlData(&gizm_pixl_data);

	// Drawing the mesh
	toDraw->draw();
}

void Gizmo() {
	if (chosenGizmo != GIZMO_NONE) {
		// Saving the camera matrix
		camMat = camera.matrix();

		// Saving the selected move matrix
		objMoveMat = kl::mat4::translate(selected->position);

		// Getting the mesh rotation/pointer
		kl::vec3 xRot;
		kl::vec3 zRot;
		kl::vec3 yRot;
		kl::mesh* gizmoMesh = nullptr;
		if (chosenGizmo == GIZMO_ROTATE) {
			xRot = kl::vec3(0, 90, 0);
			yRot = kl::vec3(90, 0, 0);
			zRot = kl::vec3(0, 0, 0);
			gizmoMesh = gizmo_rotate;
		}
		else {
			xRot = kl::vec3( 0, 0, -90);
			yRot = kl::vec3( 0, 0,   0);
			zRot = kl::vec3(90, 0,   0);
			gizmoMesh = (chosenGizmo == GIZMO_SCALE) ? gizmo_scale : gizmo_move;
		}

		// Cleaning the depth
		gpu->clearDepth();

		// Drawing the gizmos
		DrawGizmo(gizmoMesh, xRot, xCol, -3);
		DrawGizmo(gizmoMesh, yRot, yCol, -4);
		DrawGizmo(gizmoMesh, zRot, zCol, -5);
	}
}
