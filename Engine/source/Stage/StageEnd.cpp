#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Data/Entities.h"
#include "Data/Skyboxes.h"
#include "Data/Textures.h"
#include "Data/Meshes.h"
#include "Scripting/Scripting.h"


void Engine::Stage::End() {
	Engine::entities.clear();
	Engine::skyboxes.clear();
	Engine::textures.clear();
	Engine::meshes.clear();
}
