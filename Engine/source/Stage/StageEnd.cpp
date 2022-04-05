#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Data/Entities.h"
#include "View/Background.h"
#include "Scripting/Scripting.h"


void Engine::Stage::End()
{
	// Deleting entities
	Engine::entities.clear();

	// Deleting skyboxes
	Engine::Background::skyboxes.clear();

	// Gpu cleanup
	delete Engine::Render::gpu;

	// Handler cleanup
	Engine::JavaHandler::Uninit();
}
