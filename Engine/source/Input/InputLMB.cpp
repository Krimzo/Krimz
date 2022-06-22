#include "Input/Input.h"
#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Types/Camera.h"
#include "Types/Entity.h"
#include "GUI/GUI.h"


static kl::float3 offsPos;
static kl::float3 firstScal;
static kl::float3 offsRot;
static kl::float3 firstRota;

static const kl::float3 f3PosX = { 1.0f, 0.0f, 0.0f };
static const kl::float3 f3PosY = { 0.0f, 1.0f, 0.0f };
static const kl::float3 f3PosZ = { 0.0f, 0.0f, 1.0f };
static const kl::float2 f2PosX = { 1.0f, 0.0f };

kl::ray GetMouseRay() {
	kl::float2 mouseUV = Engine::window.mouse.position;
	mouseUV -= Engine::GUI::viewportPosition;
	mouseUV = kl::float2(mouseUV.x / Engine::GUI::viewportSize.x, (Engine::GUI::viewportSize.y - mouseUV.y) / Engine::GUI::viewportSize.y);
	mouseUV *= 2.0f;
	mouseUV -= 1.0f;

	kl::float4 farMousePoint = Engine::Selected::camera->matrix().inverse() * kl::float4(mouseUV, 1.0f, 1.0f);
	farMousePoint /= farMousePoint.w;

	return kl::ray(Engine::Selected::camera->position, farMousePoint.xyz - Engine::Selected::camera->position);
}

void IntersectPlanes(const kl::ray& ray, kl::float3& out1, kl::float3& out2, kl::float3& out3) {
	const kl::float3 camFor = Engine::Selected::camera->forward();
	ray.intersect(kl::plane(f3PosY, Engine::Selected::entity->position), &out1);
	ray.intersect(kl::plane((abs(camFor.x) > abs(camFor.z) ? f3PosX : f3PosZ), Engine::Selected::entity->position), &out2);
	ray.intersect(kl::plane(f3PosY, Engine::Selected::entity->position), &out3);
}

void LMBPress() {
	if (Engine::GUI::viewportFocus) {
		if (Engine::Picking::mouseIndex >= 0) {
			auto entIt = Engine::entities.begin();
			std::advance(entIt, Engine::Picking::mouseIndex);
			Engine::Selected::entity = *entIt;
			Engine::Picking::selectedIndex = Engine::Picking::mouseIndex;
		}
		else if (Engine::Picking::mouseIndex == -1) {
			Engine::Selected::entity = nullptr;
			Engine::Picking::selectedIndex = -1;
		}

		if (Engine::Selected::entity) {
			firstScal = Engine::Selected::entity->scale;
			firstRota = Engine::Selected::entity->rotation;
			Engine::Picking::heldIndex = Engine::Picking::mouseIndex;

			const kl::ray mouseRay = GetMouseRay();
			kl::float3 inter1, inter2, inter3;
			IntersectPlanes(mouseRay, inter1, inter2, inter3);
			offsPos.x = inter1.x - Engine::Selected::entity->position.x;
			offsPos.y = inter2.y - Engine::Selected::entity->position.y;
			offsPos.z = inter3.z - Engine::Selected::entity->position.z;

			kl::float3 intersectX;
			mouseRay.intersect(kl::plane(f3PosX, Engine::Selected::entity->position), &intersectX);
			kl::float2 secondVecX = kl::float2(intersectX.y, intersectX.z) - kl::float2(Engine::Selected::entity->position.y, Engine::Selected::entity->position.z);
			offsRot.x = f2PosX.angle(secondVecX, true);

			kl::float3 intersectY;
			mouseRay.intersect(kl::plane(f3PosY, Engine::Selected::entity->position), &intersectY);
			kl::float2 secondVecY = kl::float2(intersectY.z, intersectY.x) - kl::float2(Engine::Selected::entity->position.z, Engine::Selected::entity->position.x);
			offsRot.y = f2PosX.angle(secondVecY, true);

			kl::float3 intersectZ;
			mouseRay.intersect(kl::plane(f3PosZ, Engine::Selected::entity->position), &intersectZ);
			kl::float2 secondVecZ = kl::float2(intersectZ.x, intersectZ.y) - kl::float2(Engine::Selected::entity->position.x, Engine::Selected::entity->position.y);
			offsRot.z = f2PosX.angle(secondVecZ, true);
		}
	}
}

void LMBDown() {
	if (Engine::Selected::entity) {
		kl::float3 inter1, inter2, inter3;
		const kl::ray mouseRay = GetMouseRay();
		IntersectPlanes(mouseRay, inter1, inter2, inter3);

		const kl::float3 currPos = kl::float3(inter1.x, inter2.y, inter3.z) - offsPos;

		if (Engine::Selected::gizmo == Engine::Gizmo::Type::SCALE) {
			if (Engine::Picking::heldIndex == -3) {
				Engine::Selected::entity->scale.x = firstScal.x + (currPos.x - Engine::Selected::entity->position.x);
			}
			else if (Engine::Picking::heldIndex == -4) {
				Engine::Selected::entity->scale.y = firstScal.y + (currPos.y - Engine::Selected::entity->position.y);
			}
			else if (Engine::Picking::heldIndex == -5) {
				Engine::Selected::entity->scale.z = firstScal.z + (currPos.z - Engine::Selected::entity->position.z);
			}
		}
		else if (Engine::Selected::gizmo == Engine::Gizmo::Type::MOVE) {
			if (Engine::Picking::heldIndex == -3) {
				Engine::Selected::entity->position.x = currPos.x;
			}
			else if (Engine::Picking::heldIndex == -4) {
				Engine::Selected::entity->position.y = currPos.y;
			}
			else if (Engine::Picking::heldIndex == -5) {
				Engine::Selected::entity->position.z = currPos.z;
			}
		}
		else if (Engine::Selected::gizmo == Engine::Gizmo::Type::ROTATE) {
			if (Engine::Picking::heldIndex == -3) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(f3PosX, Engine::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.y, intersect.z) - kl::float2(Engine::Selected::entity->position.y, Engine::Selected::entity->position.z);
				Engine::Selected::entity->rotation.x = firstRota.x + f2PosX.angle(secondVec, true) - offsRot.x;
			}
			else if (Engine::Picking::heldIndex == -4) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(f3PosY, Engine::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.z, intersect.x) - kl::float2(Engine::Selected::entity->position.z, Engine::Selected::entity->position.x);
				Engine::Selected::entity->rotation.y = firstRota.y + f2PosX.angle(secondVec, true) - offsRot.y;
			}
			else if (Engine::Picking::heldIndex == -5) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(f3PosZ, Engine::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.x, intersect.y) - kl::float2(Engine::Selected::entity->position.x, Engine::Selected::entity->position.y);
				Engine::Selected::entity->rotation.z = firstRota.z + f2PosX.angle(secondVec, true) - offsRot.z;
			}
		}
	}
}

void LMBRelease() {
	Engine::Picking::heldIndex = -1;
}

void Engine::Input::LMB() {
	Engine::window.mouse.lmb.press = LMBPress;
	Engine::window.mouse.lmb.down = LMBDown;
	Engine::window.mouse.lmb.release = LMBRelease;
}
