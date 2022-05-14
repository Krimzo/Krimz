#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Types/Camera.h"


struct GIZMO_PS_CB {
	kl::float4 objCol;
	kl::float4 objInd;
};

void DrawGizmo(ID3D11Buffer* toDraw, const kl::float3& rot, const kl::float4& col, int index, float alterScale = 1.0f) {
	// Building the wvp matrix
	const kl::mat4 sc = kl::mat4::scale((Engine::Selected::camera->position - Engine::Selected::entity->position).length() * Engine::Gizmo::scale * alterScale);
	const kl::mat4 ro = kl::mat4::rotate(rot);
	const kl::mat4 tr = kl::mat4::translate(Engine::Selected::entity->position);
	kl::mat4 wvp = Engine::Selected::camera->matrix() * tr * ro * sc;
	Engine::Render::gpu->autoVertexCBuffer(wvp);

	// Setting the pixl data
	GIZMO_PS_CB gizm_pixl_data = {};
	gizm_pixl_data.objCol = col;
	gizm_pixl_data.objInd = float(index);
	Engine::Render::gpu->autoPixelCBuffer(gizm_pixl_data);

	// Drawing the mesh
	Engine::Render::gpu->draw(toDraw);
}

void Engine::Render::Gizmo() {
	if (Engine::Gizmo::selected != Engine::Gizmo::Type::NONE) {
		// Binding render texture
		Engine::Render::gpu->bindTargets({ Engine::Render::targetV, Engine::Picking::targetV });

		// Raster bind
		Engine::Render::gpu->bind(Engine::Rasters::solid);

		// Depth bind
		Engine::Render::gpu->bind(Engine::DepthStencil::depth);

		// Clearing the depth
		Engine::Render::gpu->clearDepth();

		// Binding gizmo shaders
		Engine::Render::gpu->bind(Engine::Shaders::gizmo);

		// Color reset
		Engine::Gizmo::colX = kl::color(205, 55, 75);
		Engine::Gizmo::colY = kl::color(115, 175, 40);
		Engine::Gizmo::colZ = kl::color(55, 120, 205);

		// Entity id check
		if (Engine::Picking::heldIndex >= -2) {
			if (Engine::Picking::mouseIndex == -3) {
				Engine::Gizmo::colX *= 1.45f;
			}
			else if (Engine::Picking::mouseIndex == -4) {
				Engine::Gizmo::colY *= 1.45f;
			}
			else if (Engine::Picking::mouseIndex == -5) {
				Engine::Gizmo::colZ *= 1.45f;
			}
		}
		else {
			if (Engine::Picking::heldIndex == -3) {
				Engine::Gizmo::colX *= 1.45f;
			}
			else if (Engine::Picking::heldIndex == -4) {
				Engine::Gizmo::colY *= 1.45f;
			}
			else if (Engine::Picking::heldIndex == -5) {
				Engine::Gizmo::colZ *= 1.45f;
			}
		}

		// Setting the rotations and mesh pointer
		kl::float3 xRot;
		kl::float3 zRot;
		kl::float3 yRot;
		ID3D11Buffer* gizmoMesh = nullptr;
		if (Engine::Gizmo::selected == Engine::Gizmo::Type::ROTATE) {
			const kl::float3 posDif = (Engine::Selected::camera->position - Engine::Selected::entity->position).normalize();
			xRot = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.z < 0.0f) ? 90.0f : -90.0f, 0.0f);
			yRot = kl::float3(90.0f, ((posDif.x < 0.0f) ? ((posDif.z < 0.0f) ? 180.0f : 270.0f) : ((posDif.z < 0.0f) ? 90.0f : 0.0f)), 0.0f);
			zRot = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.x < 0.0f) ? 180.0f : 0.0f, 0.0f);
			gizmoMesh = Engine::Gizmo::rotateM;
		}
		else {
			xRot = kl::float3(0.0f, 0.0f, -90.0f);
			yRot = kl::float3(0.0f, 0.0f, 0.0f);
			zRot = kl::float3(90.0f, 0.0f, 0.0f);
			gizmoMesh = (Engine::Gizmo::selected == Engine::Gizmo::Type::SCALE) ? Engine::Gizmo::scaleM : Engine::Gizmo::moveM;
		}

		// Drawing the gizmos
		DrawGizmo(Engine::Default::sphere->buffer, {}, kl::colors::white, -2, 0.06f);
		DrawGizmo(gizmoMesh, xRot, Engine::Gizmo::colX, -3);
		DrawGizmo(gizmoMesh, yRot, Engine::Gizmo::colY, -4);
		DrawGizmo(gizmoMesh, zRot, Engine::Gizmo::colZ, -5);
	}
}
