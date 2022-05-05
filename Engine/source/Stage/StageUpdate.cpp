#include "Stage/Stage.h"
#include "Types/Camera.h"
#include "Types/Entity.h"
#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Logging/Logging.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"
#include "GUI/GUI.h"


void Engine::Stage::Update() {
	// Time
	Engine::Time::delta = Engine::Time::timer.interval();
	Engine::Time::elapsed = Engine::Time::timer.elapsed();

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

	// Clearing the frame buffers
	static const kl::color defBackColor = kl::color(20, 20, 20);
	const kl::color& backColor = Engine::Selected::camera ? Engine::Selected::camera->color : defBackColor;
	Engine::Render::gpu->clear(backColor);
	Engine::Render::gpu->clear(Engine::Render::targetV, backColor);

	// Camera bound check
	if (Engine::Selected::camera) {
		// Rendering shadows
		Engine::Render::Shadows();

		// Viewport fix
		Engine::Render::FixViewport();

		// Skybox draw
		if (Engine::Selected::camera->skybox) {
			Engine::Render::Skybox();
		}

		// Entity render
		Engine::Render::Entities();

		// Selected postprocess
		if (Engine::Selected::entity) {
			// Outline draw
			Engine::Render::Outline();

			// Collider draw
			Engine::Render::Collider();

			// Gizmo render
			Engine::Render::Gizmo();
		}

		// Mouse object index
		Engine::Picking::ReadObjectIndex();
	}
	else {
		Engine::Picking::mouseIndex = -1;
	}

	// GUI draw
	Engine::Render::GUI();

	// Backbuffer swap
	Engine::Render::gpu->swap(Engine::Render::vSync);
}
