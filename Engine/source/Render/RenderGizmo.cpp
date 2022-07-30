#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Types/Camera.h"


struct GIZMO_PS_CB {
	kl::float4 color;
	kl::float4 index;
};

void DrawGizmo(kl::dx::buffer toDraw, const kl::float3& rot, const kl::float4& col, int index, float alterScale = 1.0f) {
	const kl::mat4 scaling = kl::mat4::scaling((Krimz::Selected::camera->position - Krimz::Selected::entity->position).length() * Krimz::Gizmo::scale * alterScale);
	const kl::mat4 rotation = kl::mat4::rotation(rot);
	const kl::mat4 translation = kl::mat4::translation(Krimz::Selected::entity->position);
	const kl::mat4 wvp = Krimz::Selected::camera->matrix() * translation * rotation * scaling;
	Krimz::gpu->autoVertexCBuffer(wvp);

	GIZMO_PS_CB gizmoPixelData = {};
	gizmoPixelData.color = col;
	gizmoPixelData.index = float(index);
	Krimz::gpu->autoPixelCBuffer(gizmoPixelData);

	Krimz::gpu->draw(toDraw);
}

void Krimz::Render::Gizmo() {
	if (Krimz::Selected::gizmo != Krimz::Gizmo::Type::NONE) {
		Krimz::gpu->bindTargets({ Krimz::Render::targetView, Krimz::Picking::targetView });
		Krimz::gpu->bind(Krimz::Rasters::solid);
		Krimz::gpu->bind(Krimz::DepthStencil::depth);
		Krimz::gpu->bind(Krimz::Shaders::gizmo);
		Krimz::gpu->clearDepth();

		Krimz::Gizmo::colors.x = kl::color(205, 55, 75);
		Krimz::Gizmo::colors.y = kl::color(115, 175, 40);
		Krimz::Gizmo::colors.z = kl::color(55, 120, 205);

		if (Krimz::Picking::heldIndex >= -2) {
			if (Krimz::Picking::mouseIndex == -3) {
				Krimz::Gizmo::colors.x *= 1.45f;
			}
			else if (Krimz::Picking::mouseIndex == -4) {
				Krimz::Gizmo::colors.y *= 1.45f;
			}
			else if (Krimz::Picking::mouseIndex == -5) {
				Krimz::Gizmo::colors.z *= 1.45f;
			}
		}
		else {
			if (Krimz::Picking::heldIndex == -3) {
				Krimz::Gizmo::colors.x *= 1.45f;
			}
			else if (Krimz::Picking::heldIndex == -4) {
				Krimz::Gizmo::colors.y *= 1.45f;
			}
			else if (Krimz::Picking::heldIndex == -5) {
				Krimz::Gizmo::colors.z *= 1.45f;
			}
		}

		kl::vector3<kl::float3> rotations;
		kl::dx::buffer gizmoMesh = nullptr;
		if (Krimz::Selected::gizmo == Krimz::Gizmo::Type::ROTATE) {
			const kl::float3 posDif = (Krimz::Selected::camera->position - Krimz::Selected::entity->position).normalize();
			rotations.x = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.z < 0.0f) ? 90.0f : -90.0f, 0.0f);
			rotations.y = kl::float3(90.0f, ((posDif.x < 0.0f) ? ((posDif.z < 0.0f) ? 180.0f : 270.0f) : ((posDif.z < 0.0f) ? 90.0f : 0.0f)), 0.0f);
			rotations.z = kl::float3((posDif.y < 0.0f) ? 180.0f : 0.0f, (posDif.x < 0.0f) ? 180.0f : 0.0f, 0.0f);
			gizmoMesh = Krimz::Meshes::Gizmo::rotate;
		}
		else {
			rotations.x = kl::float3(0.0f, 0.0f, -90.0f);
			rotations.y = kl::float3(0.0f, 0.0f, 0.0f);
			rotations.z = kl::float3(90.0f, 0.0f, 0.0f);
			gizmoMesh = (Krimz::Selected::gizmo == Krimz::Gizmo::Type::SCALE) ? Krimz::Meshes::Gizmo::scale : Krimz::Meshes::Gizmo::move;
		}

		DrawGizmo(Krimz::Meshes::Default::sphere->buffer, {}, kl::colors::white, -2, 0.06f);
		DrawGizmo(gizmoMesh, rotations.x, Krimz::Gizmo::colors.x, -3);
		DrawGizmo(gizmoMesh, rotations.y, Krimz::Gizmo::colors.y, -4);
		DrawGizmo(gizmoMesh, rotations.z, Krimz::Gizmo::colors.z, -5);
	}
}
