#include "Engine/Stage/Stage.h"
#include "Engine/Render/Render.h"
#include "Engine/Game/Game.h"
#include "Engine/View/Background.h"


void Engine::Stage::End() {
	// Deleting entities
	Engine::Game::entities.clear();

	// Deleting skyboxes
	Engine::Background::skyboxes.clear();

	// Deleting the gpu
	delete Engine::Render::gpu;
}
