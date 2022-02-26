#include "Engine/Input.h"


bool firstLmb = true;
kl::vec3 offsPos;
kl::vec3 firstSize;
kl::vec3 offsRot;
kl::vec3 firstRota;
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
				kl::vec4 farMousePoint = camera.matrix().inverse() * kl::vec4(win.mouse.normPos(win.getSize()), 1.0f, 1.0f);
				farMousePoint /= farMousePoint.w;

				// Mouse pos ray constuction
				const kl::ray mouseRay(camera.position, farMousePoint.xyz() - camera.position);

				// Getting the cam forward vector
				const kl::vec3 camFor = camera.getForward();

				// Saving the first click info
				if (firstLmb) {
					heldIndex = entityIndex;

					// Saving position offset
					offsPos.x = mouseRay.intersect(kl::plane(kl::vec3::pos_y, selected->position)).x - selected->position.x;
					offsPos.y = mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::vec3::pos_x : kl::vec3::pos_z, selected->position)).y - selected->position.y;
					offsPos.z = mouseRay.intersect(kl::plane(kl::vec3::pos_y, selected->position)).z - selected->position.z;

					// Saving the angle offset
					const kl::vec3 intersectX = mouseRay.intersect(kl::plane(kl::vec3::pos_x, selected->position));
					const kl::vec2 secondVecX = kl::vec2(intersectX.y, intersectX.z) - kl::vec2(selected->position.y, selected->position.z);
					offsRot.x = kl::vec2::pos_x.angle(secondVecX, true);
					const kl::vec3 intersectY = mouseRay.intersect(kl::plane(kl::vec3::pos_y, selected->position));
					const kl::vec2 secondVecY = kl::vec2(intersectY.z, intersectY.x) - kl::vec2(selected->position.z, selected->position.x);
					offsRot.y = kl::vec2::pos_x.angle(secondVecY, true);
					const kl::vec3 intersectZ = mouseRay.intersect(kl::plane(kl::vec3::pos_z, selected->position));
					const kl::vec2 secondVecZ = kl::vec2(intersectZ.x, intersectZ.y) - kl::vec2(selected->position.x, selected->position.y);
					offsRot.z = kl::vec2::pos_x.angle(secondVecZ, true);

					// Saving properties
					firstSize = selected->size;
					firstRota = selected->rotation;
				}
				else {
					// Intersect calculation
					const kl::vec3 currPos = kl::vec3(
						mouseRay.intersect(kl::plane(kl::vec3::pos_y, selected->position)).x,
						mouseRay.intersect(kl::plane(abs(camFor.x) > abs(camFor.z) ? kl::vec3::pos_x : kl::vec3::pos_z, selected->position)).y,
						mouseRay.intersect(kl::plane(kl::vec3::pos_y, selected->position)).z
					) - offsPos;

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
							const kl::vec3 intersect = mouseRay.intersect(kl::plane(kl::vec3::pos_x, selected->position));
							const kl::vec2 secondVec = kl::vec2(intersect.y, intersect.z) - kl::vec2(selected->position.y, selected->position.z);
							selected->rotation.x = firstRota.x + kl::vec2::pos_x.angle(secondVec, true) - offsRot.x;
						}
						else if (heldIndex == -4) {
							const kl::vec3 intersect = mouseRay.intersect(kl::plane(kl::vec3::pos_y, selected->position));
							const kl::vec2 secondVec = kl::vec2(intersect.z, intersect.x) - kl::vec2(selected->position.z, selected->position.x);
							selected->rotation.y = firstRota.y + kl::vec2::pos_x.angle(secondVec, true) - offsRot.y;
						}
						else if (heldIndex == -5) {
							const kl::vec3 intersect = mouseRay.intersect(kl::plane(kl::vec3::pos_z, selected->position));
							const kl::vec2 secondVec = kl::vec2(intersect.x, intersect.y) - kl::vec2(selected->position.x, selected->position.y);
							selected->rotation.z = firstRota.z + kl::vec2::pos_x.angle(secondVec, true) - offsRot.z;
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
