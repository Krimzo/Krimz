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
	Engine::Time::delta = float(Engine::Time::timer.interval());
	Engine::Time::elapsed = float(Engine::Time::timer.elapsed());

	if (Engine::gameRunning) {
		Engine::Physics::Update();

		Engine::Scripting::UpdateTime();
		Engine::Scripting::UpdateInput();
		Engine::Scripting::CallUpdates();
		Engine::Scripting::HandleMousePos();
		Engine::Scripting::HandleLogs();
	}

	const kl::color& clearColor = Engine::Selected::camera ? Engine::Selected::camera->color : kl::colors::gray;
	Engine::gpu->clear(clearColor);
	Engine::gpu->clear(Engine::Render::targetView, clearColor);

	if (Engine::Selected::camera) {
		Engine::Render::Shadows();
		Engine::Render::FixViewport();

		if (Engine::Selected::camera->skybox) {
			Engine::Render::Skybox();
		}

		Engine::Render::Entities();

		if (Engine::Selected::entity) {
			Engine::Render::Outline();
			Engine::Render::Collider();
			Engine::Render::Gizmo();
		}

		Engine::Picking::ReadObjectIndex();
	}
	else {
		Engine::Picking::mouseIndex = -1;
	}

	Engine::Render::GUI();

	Engine::gpu->swap(Engine::Render::vSync);
}
