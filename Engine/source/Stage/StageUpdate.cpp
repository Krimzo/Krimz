#include "Stage/Stage.h"
#include "Types/Camera.h"
#include "Types/Entity.h"
#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Logging/Logging.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"
#include "GUI/GUI.h"


void Krimz::Stage::Update() {
	Krimz::Time::delta = float(Krimz::Time::timer.interval());
	Krimz::Time::elapsed = float(Krimz::Time::timer.elapsed());

	if (Krimz::gameRunning) {
		Krimz::Physics::Update();

		Krimz::Scripting::UpdateTime();
		Krimz::Scripting::UpdateInput();
		Krimz::Scripting::CallUpdates();
		Krimz::Scripting::HandleMousePos();
		Krimz::Scripting::HandleLogs();
	}

	const kl::color& clearColor = Krimz::Selected::camera ? Krimz::Selected::camera->color : kl::colors::gray;
	Krimz::gpu->clear(clearColor);
	Krimz::gpu->clear(Krimz::Render::targetView, clearColor);

	if (Krimz::Selected::camera) {
		Krimz::Render::Shadows();
		Krimz::Render::FixViewport();

		if (Krimz::Selected::camera->skybox) {
			Krimz::Render::Skybox();
		}

		Krimz::Render::Entities();

		if (Krimz::Selected::entity) {
			Krimz::Render::Outline();
			Krimz::Render::Collider();
			Krimz::Render::Gizmo();
		}

		Krimz::Picking::ReadObjectIndex();
	}
	else {
		Krimz::Picking::mouseIndex = -1;
	}

	Krimz::Render::GUI();

	Krimz::gpu->swap(Krimz::Render::vSync);
}
