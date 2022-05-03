#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Data/Entities.h"
#include "Data/Skyboxes.h"
#include "Data/Textures.h"
#include "Data/Meshes.h"
#include "Scripting/Scripting.h"


void Engine::Stage::End() {
	// Cameras
	Engine::cameras.clear();
	Engine::Selected::camera.reset();
	Engine::Default::camera.reset();

	// Entities
	Engine::entities.clear();
	Engine::Selected::entity.reset();

	// Skyboxes/textures
	Engine::skyboxes.clear();
	Engine::textures.clear();
	Engine::Default::texture;

	// Meshes
	Engine::meshes.clear();
	Engine::Default::cube.reset();
	Engine::Default::sphere.reset();
	Engine::Default::capsule.reset();
	Engine::Default::pyramid.reset();
	Engine::Default::monke.reset();

	// Gpu
	Engine::Render::gpu.reset();
}
