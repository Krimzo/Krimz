#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Types/Camera.h"


struct GIZMO_PS_CB {
	kl::float4 color;
	kl::float4 index;
};

void DrawGizmo(kl::dx::buffer toDraw, const kl::float3& rot, const kl::float4& col, int index, float alterScale = 1.0f) {
	const kl::mat4 scaling = kl::mat4::scaling((Engine::Selected::camera->position - Engine::Selected::entity->position).length() * Engine::Gizmo::scale * alterScale);
	const kl::mat4 rotation = kl::mat4::rotation(rot);
	const kl::mat4 translation = kl::mat4::translation(Engine::Selected::entity->position);
	const kl::mat4 wvp = Engine::Selected::camera->matrix() * translation * rotation * scaling;
	Engine::gpu->autoVertexCBuffer(wvp);

	GIZMO_PS_CB gizmoPixelData = {};
	gizmoPixelData.color = col;
	gizmoPixelData.index = float(index);
	Engine::gpu->autoPixelCBuffer(gizmoPixelData);

	Engine::gpu->draw(toDraw);
}

void Engine::Render::Gizmo() {
	if (Engine::Selected::gizmo != Engine::Gizmo::Type::NONE) {
		Engine::gpu->bindTargets({ Engine::Render::targetView, Engine::Picking::targetView });
		Engine::gpu->bind(Engine::Rasters::solid);
		Engine::gpu->bind(Engine::DepthStencil::depth);
		Engine::gpu->bind(Engine::Shaders::gizmo);
		Engine::gpu->clearDepth();

		Engine::Gizmo::colors.x = kl::color(205, 55, 75);
		Engine::Gizmo::colors.y = kl::color(115, 175, 40);
		Engine::Gizmo::colors.z = kl::color(55, 120, 205);

		if (Engine::Picking::heldIndex >= -2) {
			if (Engine::Picking::mouseIndex == -3) {
				Engine::Gizmo::colors.x *= 1.45f;
			}
			else if (Engine::Picking::mouseIndex == -4) {
				Engine::Gizmo::colors.y *= 1.45f;
			}
			else if (Engine::Picking::mouseIndex == -5) {
				Engine::Gizmo::colors.z *= 1.45f;
			}
		}
		else {
			if (Engine::Picking::heldIndex == -3) {
				Engine::Gizmo::colors.x *= 1.45f;
			}
			else if (Engine::Picking::heldIndex == -4) {
				Engine::Gizmo::colors.y *= 1.45f;
			}
			else if (Engine::Picking::heldIndex == -5) {
				Engine::Gizmo::colors.z *= 1.45f;
			}
		}

		kl::vector3<kl::float3> rotations;
		kl::dx::buffer gizmoMesh = nullptr;
		if (Engine::Selected::gizmo == Engine::Gizmo::Type::ROTATE) {
			const kl::float3 posDif = (Engine::Selected::camera->position - Engine::Selected::entity->position).normalize();
			rotations.x = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.z < 0.0f) ? 90.0f : -90.0f, 0.0f);
			rotations.y = kl::float3(90.0f, ((posDif.x < 0.0f) ? ((posDif.z < 0.0f) ? 180.0f : 270.0f) : ((posDif.z < 0.0f) ? 90.0f : 0.0f)), 0.0f);
			rotations.z = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.x < 0.0f) ? 180.0f : 0.0f, 0.0f);
			gizmoMesh = Engine::Meshes::Gizmo::rotate;
		}
		else {
			rotations.x = kl::float3(0.0f, 0.0f, -90.0f);
			rotations.y = kl::float3(0.0f, 0.0f, 0.0f);
			rotations.z = kl::float3(90.0f, 0.0f, 0.0f);
			gizmoMesh = (Engine::Selected::gizmo == Engine::Gizmo::Type::SCALE) ? Engine::Meshes::Gizmo::scale : Engine::Meshes::Gizmo::move;
		}

		DrawGizmo(Engine::Meshes::Default::sphere->buffer, {}, kl::colors::white, -2, 0.06f);
		DrawGizmo(gizmoMesh, rotations.x, Engine::Gizmo::colors.x, -3);
		DrawGizmo(gizmoMesh, rotations.y, Engine::Gizmo::colors.y, -4);
		DrawGizmo(gizmoMesh, rotations.z, Engine::Gizmo::colors.z, -5);
	}
}
