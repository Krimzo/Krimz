#include "Engine/Engine.h"


void Engine::Stage::End() {
	// Deleting entities
	Engine::Game::entities.clear();

	// Deleting skyboxes
	Engine::Background::skyboxes.clear();

	// Deleting the gpu
	delete Engine::Render::gpu;
}
