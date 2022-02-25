#include "Engine/Engine.h"


const kl::vec4 xCol = kl::color(205,  55,  75);
const kl::vec4 yCol = kl::color(115, 175,  40);
const kl::vec4 zCol = kl::color( 55, 120, 205);

const float gizmoScale = 0.2f;

void DrawGizmo(kl::mesh* toDraw, const kl::vec3& rot, const kl::vec4& col, int index) {
	// Building the wvp matrix
	const kl::mat4 sc = kl::mat4::scale((camera.position - selected->position).length() * gizmoScale);
	const kl::mat4 ro = kl::mat4::rotate(rot);
	const kl::mat4 tr = kl::mat4::translate(selected->position);
	kl::mat4 wvp = camera.matrix() * tr * ro * sc;
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
		// Setting the rotations and mesh pointer
		kl::vec3 xRot;
		kl::vec3 zRot;
		kl::vec3 yRot;
		kl::mesh* gizmoMesh = nullptr;
		if (chosenGizmo == GIZMO_ROTATE) {
			const kl::vec3 posDif = (camera.position - selected->position).normalize();
			xRot = kl::vec3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.z < 0.0f) ? 90.0f : -90.0f, 0.0f);
			yRot = kl::vec3(90.0f, ((posDif.x < 0.0f) ? ((posDif.z < 0.0f) ? 180.0f : 270.0f) : ((posDif.z < 0.0f) ? 90.0f : 0.0f)), 0.0f);
			zRot = kl::vec3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.x < 0.0f) ? 180.0f : 0.0f, 0.0f);
			gizmoMesh = gizmo_rotate;
		}
		else {
			xRot = kl::vec3( 0, 0, -90);
			yRot = kl::vec3( 0, 0,   0);
			zRot = kl::vec3(90, 0,   0);
			gizmoMesh = (chosenGizmo == GIZMO_SCALE) ? gizmo_scale : gizmo_move;
		}

		// Clearing the depth
		gpu->clearDepth();

		// Drawing the gizmos
		DrawGizmo(gizmoMesh, xRot, xCol, -3);
		DrawGizmo(gizmoMesh, yRot, yCol, -4);
		DrawGizmo(gizmoMesh, zRot, zCol, -5);
	}
}
