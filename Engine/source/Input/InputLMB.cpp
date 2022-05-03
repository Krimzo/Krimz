#include "Input/Input.h"
#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Data/Entities.h"
#include "GUI/GUI.h"


kl::float3 offsPos;
kl::float3 firstScal;
kl::float3 offsRot;
kl::float3 firstRota;

kl::ray GetMouseRay() {
	// Mouse UV
	kl::float2 mouseUV = Engine::win.mouse.position;
	mouseUV -= Engine::GUI::viewportPos;
	mouseUV = kl::float2(mouseUV.x / Engine::GUI::viewportSize.x, (Engine::GUI::viewportSize.y - mouseUV.y) / Engine::GUI::viewportSize.y);
	mouseUV *= 2.0f;
	mouseUV -= 1.0f;

	// Far mouse point
	kl::float4 farMousePoint = Engine::Selected::camera->matrix().inverse() * kl::float4(mouseUV, 1.0f, 1.0f);
	farMousePoint /= farMousePoint.w;

	// Mouse pos ray constuction
	return kl::ray(Engine::Selected::camera->position, farMousePoint.xyz() - Engine::Selected::camera->position);
}

void LMBPress() {
	if (Engine::GUI::viewportFocus) {
		// Index test
		if (Engine::Picking::mouseIndex >= 0) {
			auto entIt = Engine::entities.begin();
			std::advance(entIt, Engine::Picking::mouseIndex);
			Engine::Selected::entity = *entIt;
			Engine::Picking::selectedInd = Engine::Picking::mouseIndex;
		}
		else if (Engine::Picking::mouseIndex == -1) {
			Engine::Selected::entity = nullptr;
			Engine::Picking::selectedInd = -1;
		}

		// Selected test
		if (Engine::Selected::entity) {
			// Saving
			firstScal = Engine::Selected::entity->scale;
			firstRota = Engine::Selected::entity->rotation;
			Engine::Picking::heldIndex = Engine::Picking::mouseIndex;

			// Intersection
			kl::float3 inter1, inter2, inter3;
			const kl::ray mouseRay = GetMouseRay();
			const kl::float3 camFor = Engine::Selected::camera->getForward();
			mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Selected::entity->position), &inter1);
			mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::float3::pos_x : kl::float3::pos_z, Engine::Selected::entity->position), &inter2);
			mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Selected::entity->position), &inter3);

			// Saving position offset
			offsPos.x = inter1.x - Engine::Selected::entity->position.x;
			offsPos.y = inter2.y - Engine::Selected::entity->position.y;
			offsPos.z = inter3.z - Engine::Selected::entity->position.z;

			// X
			kl::float3 intersectX;
			mouseRay.intersect(kl::plane(kl::float3::pos_x, Engine::Selected::entity->position), &intersectX);
			kl::float2 secondVecX = kl::float2(intersectX.y, intersectX.z) - kl::float2(Engine::Selected::entity->position.y, Engine::Selected::entity->position.z);
			offsRot.x = kl::float2::pos_x.angle(secondVecX, true);

			// Y
			kl::float3 intersectY;
			mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Selected::entity->position), &intersectY);
			kl::float2 secondVecY = kl::float2(intersectY.z, intersectY.x) - kl::float2(Engine::Selected::entity->position.z, Engine::Selected::entity->position.x);
			offsRot.y = kl::float2::pos_x.angle(secondVecY, true);

			// Z
			kl::float3 intersectZ;
			mouseRay.intersect(kl::plane(kl::float3::pos_z, Engine::Selected::entity->position), &intersectZ);
			kl::float2 secondVecZ = kl::float2(intersectZ.x, intersectZ.y) - kl::float2(Engine::Selected::entity->position.x, Engine::Selected::entity->position.y);
			offsRot.z = kl::float2::pos_x.angle(secondVecZ, true);
		}
	}
}

void LMBDown() {
	if (Engine::Selected::entity) {
		// Intersection
		kl::float3 inter1, inter2, inter3;
		const kl::ray mouseRay = GetMouseRay();
		const kl::float3 camFor = Engine::Selected::camera->getForward();
		mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Selected::entity->position), &inter1);
		mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::float3::pos_x : kl::float3::pos_z, Engine::Selected::entity->position), &inter2);
		mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Selected::entity->position), &inter3);

		// Current position calculation
		const kl::float3 currPos = kl::float3(inter1.x, inter2.y, inter3.z) - offsPos;

		// Entity edit
		if (Engine::Gizmo::selected == Engine::Gizmo::Type::SCALE) {
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
		else if (Engine::Gizmo::selected == Engine::Gizmo::Type::MOVE) {
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
		else if (Engine::Gizmo::selected == Engine::Gizmo::Type::ROTATE) {
			if (Engine::Picking::heldIndex == -3) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(kl::float3::pos_x, Engine::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.y, intersect.z) - kl::float2(Engine::Selected::entity->position.y, Engine::Selected::entity->position.z);
				Engine::Selected::entity->rotation.x = firstRota.x + kl::float2::pos_x.angle(secondVec, true) - offsRot.x;
			}
			else if (Engine::Picking::heldIndex == -4) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.z, intersect.x) - kl::float2(Engine::Selected::entity->position.z, Engine::Selected::entity->position.x);
				Engine::Selected::entity->rotation.y = firstRota.y + kl::float2::pos_x.angle(secondVec, true) - offsRot.y;
			}
			else if (Engine::Picking::heldIndex == -5) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(kl::float3::pos_z, Engine::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.x, intersect.y) - kl::float2(Engine::Selected::entity->position.x, Engine::Selected::entity->position.y);
				Engine::Selected::entity->rotation.z = firstRota.z + kl::float2::pos_x.angle(secondVec, true) - offsRot.z;
			}
		}
	}
}

void LMBRelease() {
	Engine::Picking::heldIndex = -1;
}

void Engine::Input::LMB() {
	Engine::win.mouse.lmb.press = LMBPress;
	Engine::win.mouse.lmb.down = LMBDown;
	Engine::win.mouse.lmb.release = LMBRelease;
}
