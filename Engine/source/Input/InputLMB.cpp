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
	kl::float4 farMousePoint = Engine::Render::camera->matrix().inverse() * kl::float4(mouseUV, 1.0f, 1.0f);
	farMousePoint /= farMousePoint.w;

	// Mouse pos ray constuction
	return kl::ray(Engine::Render::camera->position, farMousePoint.xyz() - Engine::Render::camera->position);
}

void LMBPress() {
	if (Engine::GUI::viewportFocus) {
		// Index test
		if (Engine::Picking::mouseIndex >= 0) {
			auto entIt = Engine::entities.begin();
			std::advance(entIt, Engine::Picking::mouseIndex);
			Engine::Picking::selected = entIt->get();
			Engine::Picking::selectedInd = Engine::Picking::mouseIndex;
		}
		else if (Engine::Picking::mouseIndex == -1) {
			Engine::Picking::selected = nullptr;
			Engine::Picking::selectedInd = -1;
		}

		// Selected test
		if (Engine::Picking::selected) {
			// Saving
			firstScal = Engine::Picking::selected->scale;
			firstRota = Engine::Picking::selected->rotation;
			Engine::Picking::heldIndex = Engine::Picking::mouseIndex;

			// Intersection
			kl::float3 inter1, inter2, inter3;
			const kl::ray mouseRay = GetMouseRay();
			const kl::float3 camFor = Engine::Render::camera->getForward();
			mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Picking::selected->position), &inter1);
			mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::float3::pos_x : kl::float3::pos_z, Engine::Picking::selected->position), &inter2);
			mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Picking::selected->position), &inter3);

			// Saving position offset
			offsPos.x = inter1.x - Engine::Picking::selected->position.x;
			offsPos.y = inter2.y - Engine::Picking::selected->position.y;
			offsPos.z = inter3.z - Engine::Picking::selected->position.z;

			// X
			kl::float3 intersectX;
			mouseRay.intersect(kl::plane(kl::float3::pos_x, Engine::Picking::selected->position), &intersectX);
			kl::float2 secondVecX = kl::float2(intersectX.y, intersectX.z) - kl::float2(Engine::Picking::selected->position.y, Engine::Picking::selected->position.z);
			offsRot.x = kl::float2::pos_x.angle(secondVecX, true);

			// Y
			kl::float3 intersectY;
			mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Picking::selected->position), &intersectY);
			kl::float2 secondVecY = kl::float2(intersectY.z, intersectY.x) - kl::float2(Engine::Picking::selected->position.z, Engine::Picking::selected->position.x);
			offsRot.y = kl::float2::pos_x.angle(secondVecY, true);

			// Z
			kl::float3 intersectZ;
			mouseRay.intersect(kl::plane(kl::float3::pos_z, Engine::Picking::selected->position), &intersectZ);
			kl::float2 secondVecZ = kl::float2(intersectZ.x, intersectZ.y) - kl::float2(Engine::Picking::selected->position.x, Engine::Picking::selected->position.y);
			offsRot.z = kl::float2::pos_x.angle(secondVecZ, true);
		}
	}
}

void LMBDown() {
	if (Engine::Picking::selected) {
		// Intersection
		kl::float3 inter1, inter2, inter3;
		const kl::ray mouseRay = GetMouseRay();
		const kl::float3 camFor = Engine::Render::camera->getForward();
		mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Picking::selected->position), &inter1);
		mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::float3::pos_x : kl::float3::pos_z, Engine::Picking::selected->position), &inter2);
		mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Picking::selected->position), &inter3);

		// Current position calculation
		const kl::float3 currPos = kl::float3(inter1.x, inter2.y, inter3.z) - offsPos;

		// Entity edit
		if (Engine::Gizmo::selected == Engine::Gizmo::Type::SCALE) {
			if (Engine::Picking::heldIndex == -3) {
				Engine::Picking::selected->scale.x = firstScal.x + (currPos.x - Engine::Picking::selected->position.x);
			}
			else if (Engine::Picking::heldIndex == -4) {
				Engine::Picking::selected->scale.y = firstScal.y + (currPos.y - Engine::Picking::selected->position.y);
			}
			else if (Engine::Picking::heldIndex == -5) {
				Engine::Picking::selected->scale.z = firstScal.z + (currPos.z - Engine::Picking::selected->position.z);
			}
		}
		else if (Engine::Gizmo::selected == Engine::Gizmo::Type::MOVE) {
			if (Engine::Picking::heldIndex == -3) {
				Engine::Picking::selected->position.x = currPos.x;
			}
			else if (Engine::Picking::heldIndex == -4) {
				Engine::Picking::selected->position.y = currPos.y;
			}
			else if (Engine::Picking::heldIndex == -5) {
				Engine::Picking::selected->position.z = currPos.z;
			}
		}
		else if (Engine::Gizmo::selected == Engine::Gizmo::Type::ROTATE) {
			if (Engine::Picking::heldIndex == -3) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(kl::float3::pos_x, Engine::Picking::selected->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.y, intersect.z) - kl::float2(Engine::Picking::selected->position.y, Engine::Picking::selected->position.z);
				Engine::Picking::selected->rotation.x = firstRota.x + kl::float2::pos_x.angle(secondVec, true) - offsRot.x;
			}
			else if (Engine::Picking::heldIndex == -4) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(kl::float3::pos_y, Engine::Picking::selected->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.z, intersect.x) - kl::float2(Engine::Picking::selected->position.z, Engine::Picking::selected->position.x);
				Engine::Picking::selected->rotation.y = firstRota.y + kl::float2::pos_x.angle(secondVec, true) - offsRot.y;
			}
			else if (Engine::Picking::heldIndex == -5) {
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(kl::float3::pos_z, Engine::Picking::selected->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.x, intersect.y) - kl::float2(Engine::Picking::selected->position.x, Engine::Picking::selected->position.y);
				Engine::Picking::selected->rotation.z = firstRota.z + kl::float2::pos_x.angle(secondVec, true) - offsRot.z;
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
