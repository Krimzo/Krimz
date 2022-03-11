#include "Engine/Engine.h"


void End() {
	// Delete buffers
	delete outlineBuff;

	// Deleting entities
	entities.clear();

	// Deleting skyboxes
	skyboxes.clear();

	// Deleting the gpu
	delete gpu;
}
