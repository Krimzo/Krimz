#include "Engine/Engine.h"


void LMB();

void Input() {
	// Exit
	if (win.keys.esc) win.stop();

	// Movement
	if (win.keys.w) camera.moveForward(deltaT);
	if (win.keys.s) camera.moveBack(deltaT);
	if (win.keys.d) camera.moveRight(deltaT);
	if (win.keys.a) camera.moveLeft(deltaT);
	if (win.keys.space) camera.moveUp(deltaT);
	if (win.keys.c) camera.moveDown(deltaT);

	// Speed
	if (win.keys.shift) {
		camera.speed = 5;
	}
	else {
		camera.speed = 2;
	}

	// Camera rotation
	static bool camMoving = false;
	if (win.mouse.rmb) {
		// Getting the frame center
		const kl::ivec2 frameCenter = win.getCenter();

		// Fixing the camera jump on the first click
		if (!camMoving) {
			win.mouse.position = frameCenter;
		}

		// Saving info
		win.mouse.hide();
		camMoving = true;

		// Updating the cam
		camera.rotate(win.mouse.position, frameCenter);
		win.mouse.move(frameCenter);
	}
	else {
		// Saving info
		win.mouse.show();
		camMoving = false;
	}

	// Gizmo setting
	static bool num1WasDown = false;
	if (win.keys.num1) {
		if (!num1WasDown) {
			if (chosenGizmo == GIZMO_SCALE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_SCALE;
			}
		}
		num1WasDown = true;
	}
	else {
		num1WasDown = false;
	}
	static bool num2WasDown = false;
	if (win.keys.num2) {
		if (!num2WasDown) {
			if (chosenGizmo == GIZMO_MOVE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_MOVE;
			}
		}
		num2WasDown = true;
	}
	else {
		num2WasDown = false;
	}
	static bool num3WasDown = false;
	if (win.keys.num3) {
		if (!num3WasDown) {
			if (chosenGizmo == GIZMO_ROTATE) {
				chosenGizmo = GIZMO_NONE;
			}
			else {
				chosenGizmo = GIZMO_ROTATE;
			}
		}
		num3WasDown = true;
	}
	else {
		num3WasDown = false;
	}

	// LMB
	LMB();
}


bool firstLmb = true;
int lastInd = -1;
kl::vec3 offsPos;
kl::vec3 firstSize;
kl::vec3 offsRot;
kl::vec3 firstRota;
void LMB() {
	if (win.mouse.lmb) {
		// Mouse over GUI check
		if (!ImGui::GetIO().WantCaptureMouse) {
			// Reading the index
			const int ind = gpu->getIndex(win.mouse.position);

			// Index test
			if (ind >= 0 && firstLmb) {
				selected = entities[ind];
			}
			else if (ind >= -2 && firstLmb) {
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
					lastInd = ind;

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
						if (lastInd == -3) {
							selected->size.x = firstSize.x + (currPos.x - selected->position.x);
						}
						else if (lastInd == -4) {
							selected->size.y = firstSize.y + (currPos.y - selected->position.y);
						}
						else if (lastInd == -5) {
							selected->size.z = firstSize.z + (currPos.z - selected->position.z);
						}
					}
					else if (chosenGizmo == GIZMO_MOVE) {
						if (lastInd == -3) {
							selected->position.x = currPos.x;
						}
						else if (lastInd == -4) {
							selected->position.y = currPos.y;
						}
						else if (lastInd == -5) {
							selected->position.z = currPos.z;
						}
					}
					else if (chosenGizmo == GIZMO_ROTATE) {
						if (lastInd == -3) {
							const kl::vec3 intersect = mouseRay.intersect(kl::plane(kl::vec3::pos_x, selected->position));
							const kl::vec2 secondVec = kl::vec2(intersect.y, intersect.z) - kl::vec2(selected->position.y, selected->position.z);
							selected->rotation.x = firstRota.x + kl::vec2::pos_x.angle(secondVec, true) - offsRot.x;
						}
						else if (lastInd == -4) {
							const kl::vec3 intersect = mouseRay.intersect(kl::plane(kl::vec3::pos_y, selected->position));
							const kl::vec2 secondVec = kl::vec2(intersect.z, intersect.x) - kl::vec2(selected->position.z, selected->position.x);
							selected->rotation.y = firstRota.y + kl::vec2::pos_x.angle(secondVec, true) - offsRot.y;
						}
						else if (lastInd == -5) {
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
		lastInd = -1;
	}
}
