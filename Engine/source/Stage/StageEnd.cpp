#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Data/Entities.h"
#include "View/Background.h"


void Engine::Stage::End() {
	// Deleting entities
	Engine::entities.clear();

	// Deleting skyboxes
	Engine::Background::skyboxes.clear();

	// Deleting the gpu
	delete Engine::Render::gpu;
}
