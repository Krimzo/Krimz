#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Data/Entities.h"
#include "Stage/Stage.h"
#include "Editor/Picking.h"
#include "Logging/Logging.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"
#include "GUI/GUI.h"


void Engine::Stage::Update() {
	// Time
	Engine::Time::delta = Engine::Time::timer.interval();
	Engine::Time::elapsed = Engine::Time::timer.elapsed();

	// Clearing the buffers
	Engine::Render::gpu->clear(Engine::Background::color);
	Engine::Render::gpu->clear(Engine::Render::targetV, Engine::Background::color);

	// Game
	if (Engine::gameRunning) {
		// Physics
		Engine::Physics::Update();

		// Scripts
		Engine::Scripting::UpdateTime();
		Engine::Scripting::UpdateInput();
		Engine::Scripting::CallUpdates();
		Engine::Scripting::HandleMousePos();
		Engine::Scripting::HandleLogs();
	}

	// Rendering shadows
	Engine::Render::Shadows();

	// Viewport fix
	Engine::Render::FixViewport();

	// Skybox draw
	if (Engine::Background::skybox) {
		Engine::Render::Skybox();
	}

	// Entity render
	Engine::Render::Entities();

	// Selected postprocess
	if (Engine::Picking::selected) {
		// Outline draw
		Engine::Render::Outline();

		// Collider draw
		Engine::Render::Collider();

		// Gizmo render
		Engine::Render::Gizmo();
	}

	// Mouse object index
	Engine::Picking::ReadObjectIndex();

	// GUI draw
	Engine::Render::GUI();

	// Backbuffer swap
	Engine::Render::gpu->swap(Engine::Render::vSync);
}
