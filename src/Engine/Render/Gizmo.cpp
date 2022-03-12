#include "Engine/Engine.h"


const float gizmoScale = 0.2f;

void DrawGizmo(ID3D11Buffer* toDraw, const kl::float3& rot, const kl::float4& col, int index) {
	// Binding gizmo shaders
	gpu->bind(gizmo_vtx);
	gpu->bind(gizmo_pxl);
	gpu->bindVertCBuff(gizmo_vtx_cb, 0);
	gpu->bindPixlCBuff(gizmo_pxl_cb, 0);

	// Building the wvp matrix
	const kl::mat4 sc = kl::mat4::scale((camera.position - selected->position).length() * gizmoScale);
	const kl::mat4 ro = kl::mat4::rotate(rot);
	const kl::mat4 tr = kl::mat4::translate(selected->position);
	kl::mat4 wvp = camera.matrix() * tr * ro * sc;
	gpu->setBuffData(gizmo_vtx_cb, &wvp);

	// Setting the pixl data
	GIZM_PS_CB gizm_pixl_data = {};
	gizm_pixl_data.objCol = col;
	gizm_pixl_data.objInd = float(index);
	gpu->setBuffData(gizmo_pxl_cb, &gizm_pixl_data);

	// Drawing the mesh
	gpu->draw(toDraw);
}

void Gizmo() {
	if (chosenGizmo != GIZMO_NONE) {
		// Setting the rotations and mesh pointer
		kl::float3 xRot;
		kl::float3 zRot;
		kl::float3 yRot;
		ID3D11Buffer* gizmoMesh = nullptr;
		if (chosenGizmo == GIZMO_ROTATE) {
			const kl::float3 posDif = (camera.position - selected->position).normalize();
			xRot = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.z < 0.0f) ? 90.0f : -90.0f, 0.0f);
			yRot = kl::float3(90.0f, ((posDif.x < 0.0f) ? ((posDif.z < 0.0f) ? 180.0f : 270.0f) : ((posDif.z < 0.0f) ? 90.0f : 0.0f)), 0.0f);
			zRot = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.x < 0.0f) ? 180.0f : 0.0f, 0.0f);
			gizmoMesh = gizmo_rotate;
		}
		else {
			xRot = kl::float3( 0, 0, -90);
			yRot = kl::float3( 0, 0,   0);
			zRot = kl::float3(90, 0,   0);
			gizmoMesh = (chosenGizmo == GIZMO_SCALE) ? gizmo_scale : gizmo_move;
		}

		// Binding internal
		gpu->bindInternal({ pickingTargetV });

		// Clearing the depth
		gpu->clearDepth();

		// Drawing the gizmos
		DrawGizmo(gizmoMesh, xRot, gizmoColX, -3);
		DrawGizmo(gizmoMesh, yRot, gizmoColY, -4);
		DrawGizmo(gizmoMesh, zRot, gizmoColZ, -5);
	}
}
