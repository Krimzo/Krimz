#include "Engine/Input.h"


bool firstLmb = true;
kl::float3 offsPos;
kl::float3 firstSize;
kl::float3 offsRot;
kl::float3 firstRota;
void LMBInput() {
	if (win.mouse.lmb) {
		// Mouse over GUI check
		if (!ImGui::GetIO().WantCaptureMouse) {
			// Index test
			if (entityIndex >= 0 && firstLmb) {
				selected = entities[entityIndex];
			}
			else if (entityIndex >= -2 && firstLmb) {
				selected = nullptr;
			}
			else if (selected) {
				// Far mouse point calculation
				kl::float4 farMousePoint = camera.matrix().inverse() * kl::float4(win.mouse.normPos(win.getSize()), 1.0f, 1.0f);
				farMousePoint /= farMousePoint.w;

				// Mouse pos ray constuction
				const kl::ray mouseRay(camera.position, farMousePoint.xyz() - camera.position);

				// Getting the cam forward vector
				const kl::float3 camFor = camera.getForward();

				// Saving the first click info
				if (firstLmb) {
					heldIndex = entityIndex;

					// Intersection
					kl::float3 inter1;
					kl::float3 inter2;
					kl::float3 inter3;
					mouseRay.intersect(kl::plane(kl::float3::pos_y, selected->position), &inter1);
					mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::float3::pos_x : kl::float3::pos_z, selected->position), &inter2);
					mouseRay.intersect(kl::plane(kl::float3::pos_y, selected->position), &inter3);

					// Saving position offset
					offsPos.x = inter1.x - selected->position.x;
					offsPos.y = inter2.y - selected->position.y;
					offsPos.z = inter3.z - selected->position.z;

					// Saving the angle offset
					kl::float3 intersectX;
					mouseRay.intersect(kl::plane(kl::float3::pos_x, selected->position), &intersectX);
					kl::float2 secondVecX = kl::float2(intersectX.y, intersectX.z) - kl::float2(selected->position.y, selected->position.z);
					offsRot.x = kl::float2::pos_x.angle(secondVecX, true);
					kl::float3 intersectY;
					mouseRay.intersect(kl::plane(kl::float3::pos_y, selected->position), &intersectY);
					kl::float2 secondVecY = kl::float2(intersectY.z, intersectY.x) - kl::float2(selected->position.z, selected->position.x);
					offsRot.y = kl::float2::pos_x.angle(secondVecY, true);
					kl::float3 intersectZ;
					mouseRay.intersect(kl::plane(kl::float3::pos_z, selected->position), &intersectZ);
					kl::float2 secondVecZ = kl::float2(intersectZ.x, intersectZ.y) - kl::float2(selected->position.x, selected->position.y);
					offsRot.z = kl::float2::pos_x.angle(secondVecZ, true);

					// Saving properties
					firstSize = selected->size;
					firstRota = selected->rotation;
				}
				else {
					// Intersection
					kl::float3 inter1;
					kl::float3 inter2;
					kl::float3 inter3;
					mouseRay.intersect(kl::plane(kl::float3::pos_y, selected->position), &inter1);
					mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::float3::pos_x : kl::float3::pos_z, selected->position), &inter2);
					mouseRay.intersect(kl::plane(kl::float3::pos_y, selected->position), &inter3);

					// Current position calculation
					const kl::float3 currPos = kl::float3(inter1.x, inter2.y, inter3.z) - offsPos;

					// Entity edit
					if (chosenGizmo == GIZMO_SCALE) {
						if (heldIndex == -3) {
							selected->size.x = firstSize.x + (currPos.x - selected->position.x);
						}
						else if (heldIndex == -4) {
							selected->size.y = firstSize.y + (currPos.y - selected->position.y);
						}
						else if (heldIndex == -5) {
							selected->size.z = firstSize.z + (currPos.z - selected->position.z);
						}
					}
					else if (chosenGizmo == GIZMO_MOVE) {
						if (heldIndex == -3) {
							selected->position.x = currPos.x;
						}
						else if (heldIndex == -4) {
							selected->position.y = currPos.y;
						}
						else if (heldIndex == -5) {
							selected->position.z = currPos.z;
						}
					}
					else if (chosenGizmo == GIZMO_ROTATE) {
						if (heldIndex == -3) {
							kl::float3 intersect;
							mouseRay.intersect(kl::plane(kl::float3::pos_x, selected->position), &intersect);
							kl::float2 secondVec = kl::float2(intersect.y, intersect.z) - kl::float2(selected->position.y, selected->position.z);
							selected->rotation.x = firstRota.x + kl::float2::pos_x.angle(secondVec, true) - offsRot.x;
						}
						else if (heldIndex == -4) {
							kl::float3 intersect;
							mouseRay.intersect(kl::plane(kl::float3::pos_y, selected->position), &intersect);
							kl::float2 secondVec = kl::float2(intersect.z, intersect.x) - kl::float2(selected->position.z, selected->position.x);
							selected->rotation.y = firstRota.y + kl::float2::pos_x.angle(secondVec, true) - offsRot.y;
						}
						else if (heldIndex == -5) {
							kl::float3 intersect;
							mouseRay.intersect(kl::plane(kl::float3::pos_z, selected->position), &intersect);
							kl::float2 secondVec = kl::float2(intersect.x, intersect.y) - kl::float2(selected->position.x, selected->position.y);
							selected->rotation.z = firstRota.z + kl::float2::pos_x.angle(secondVec, true) - offsRot.z;
						}
					}
				}
			}
			firstLmb = false;
		}
	}
	else {
		firstLmb = true;
		heldIndex = -1;
	}
}
