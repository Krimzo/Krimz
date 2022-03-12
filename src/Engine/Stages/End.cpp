#include "Engine/Engine.h"


void End() {
	// Deleting entities
	entities.clear();

	// Deleting skyboxes
	skyboxes.clear();

	// Deleting the gpu
	delete gpu;
}
