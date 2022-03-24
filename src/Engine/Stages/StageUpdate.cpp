#include "Engine/Stage/Stage.h"
#include "Engine/Render/Render.h"
#include "Engine/Render/DStates.h"
#include "Engine/View/Background.h"
#include "Engine/Game/Game.h"
#include "Engine/Window/Window.h"
#include "Engine/Input/Picking.h"
#include "Engine/Time/Time.h"
#include "Engine/GUI/GUIDraw.h"
#include "Engine/GUI/GUIProperties.h"
#include "Engine/Scripting/Scripting.h"


int GetIndex(const kl::int2& pos);

void Engine::Stage::Update() {
	// Time
	Engine::Time::delta = Engine::Time::timer.interval();
	Engine::Time::elapsed = Engine::Time::timer.elapsed();

	// Clearing the buffers
	Engine::Render::gpu->clear(Engine::Background::color);

	// ImGui draw start
	Engine::GUI::StartDraw();

	// Gui before viewport
	Engine::Render::GUIBefore();

	// Game
	if (Engine::Game::running) {
		// Physics
		for (int i = 0; i < Engine::Game::entities.size(); i++) {
			Engine::Game::entities[i]->upPhys(Engine::Time::delta);
		}
		
		// Scripts
		Engine::Scripting::UpdateTime();
		Engine::Scripting::CallUpdates();
	}

	// Rendering shadows
	Engine::Render::Shadows();

	/* Viewport render */ {
		// Viewport fix
		Engine::Render::gpu->viewport(Engine::GUI::viewportPos, Engine::GUI::viewportSize);
		Engine::Render::camera.aspect = float(Engine::GUI::viewportSize.x) / Engine::GUI::viewportSize.y;

		// Skybox draw
		if (Engine::Background::skybox) {
			Engine::Render::gpu->bind(Engine::DepthStencil::disabled);
			Engine::Background::skybox->render(Engine::Render::camera.matrix());
			Engine::Render::gpu->bind(Engine::DepthStencil::depth);
		}

		// Entity render
		Engine::Render::Entity();

		// Selected postprocess
		if (Engine::Picking::selected) {
			// Outline draw
			Engine::Render::Outline();

			// Gizmo render
			Engine::Render::Gizmo();
		}

		// Mouse index
		Engine::Picking::mouseIndex = GetIndex(Engine::Window::win.mouse.position);
	}

	// Gui after the viewport
	Engine::Render::GUIAfter();

	// ImGui draw end
	Engine::GUI::EndDraw();

	// Backbuffer swap
	Engine::Render::gpu->swap(true);
}

// Returns the picking index
int GetIndex(const kl::int2& pos) {
	// Checking if the pos is in frame
	if (pos.x >= 0 && pos.x < Engine::Window::win.getSize().x && pos.y >= 0 && pos.y < Engine::Window::win.getSize().y) {
		// Copying the index texture pixel
		D3D11_BOX srcBox = {};
		srcBox.left = pos.x;
		srcBox.right = srcBox.left + 1;
		srcBox.top = pos.y;
		srcBox.bottom = srcBox.top + 1;
		srcBox.front = 0;
		srcBox.back = 1;
		Engine::Render::gpu->con()->CopySubresourceRegion(
			Engine::Picking::textureStag, 0, 0, 0, 0, Engine::Picking::texture, 0, &srcBox);

		// Mapping the staging texture
		D3D11_MAPPED_SUBRESOURCE msr = {};
		Engine::Render::gpu->con()->Map(Engine::Picking::textureStag, 0, D3D11_MAP_READ, NULL, &msr);

		// Reading the bytes
		float index = 0;
		memcpy(&index, msr.pData, sizeof(float));

		// Unmapping the staging texture
		Engine::Render::gpu->con()->Unmap(Engine::Picking::textureStag, NULL);

		// Getting the index
		return int(index);
	}
	return -1;
}
