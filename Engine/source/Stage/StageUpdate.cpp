#include "Stage/Stage.h"
#include "Render/Render.h"
#include "View/Background.h"
#include "Data/Entities.h"
#include "Stage/Stage.h"
#include "Utility/Window.h"
#include "Input/Picking.h"
#include "Utility/Time.h"
#include "GUI/GUI.h"
#include "Logging/Logging.h"
#include "Scripting/Scripting.h"


void Engine::Stage::Update()
{
	// Time
	Engine::Time::delta = Engine::Time::timer.interval();
	Engine::Time::elapsed = Engine::Time::timer.elapsed();

	// Clearing the buffers
	Engine::Render::gpu->clear(Engine::Background::color);

	// Game
	if (Engine::gameRunning)
	{
		// Physics
		for (int i = 0; i < Engine::entities.size(); i++)
			Engine::entities[i]->upPhys(Engine::Time::delta);

		// Scripts
		Engine::Scripting::UpdateTime();
		Engine::Scripting::CallUpdates();
		Engine::Scripting::HandleLogs();
	}

	// Rendering shadows
	Engine::Render::Shadows();

	// Viewport fix
	Engine::Render::gpu->viewport(Engine::GUI::viewportPos, Engine::GUI::viewportSize);
	Engine::Render::camera.aspect = float(Engine::GUI::viewportSize.x) / Engine::GUI::viewportSize.y;

	// Skybox draw
	if (Engine::Background::skybox)
	{
		Engine::Render::gpu->bind(Engine::DepthStencil::disabled);
		Engine::Background::skybox->render(Engine::Render::camera.matrix());
		Engine::Render::gpu->bind(Engine::DepthStencil::depth);
	}

	// Entity render
	Engine::Render::Entity();

	// Selected postprocess
	if (Engine::Picking::selected)
	{
		// Outline draw
		Engine::Render::Outline();

		// Gizmo render
		Engine::Render::Gizmo();
	}

	// Mouse object index
	Engine::Picking::ReadObjectIndex();

	// GUI draw
	Engine::Render::GUI();

	// Backbuffer swap
	Engine::Render::gpu->swap(true);
}
