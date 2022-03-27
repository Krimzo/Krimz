#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Game/Entity.h"
#include "Engine/Game/Game.h"
#include "Engine/Input/Picking.h"
#include "Engine/Time/Time.h"
#include "Engine/Scripting/Scripting.h"


std::vector<Engine::Game::Entity> savedEntities;

void Engine::GUI::Viewport() {
	// Window draw
	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground)) {
		// Focuse save
		Engine::GUI::viewportFocus = ImGui::IsWindowHovered();
		
		// Play button
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x * 0.5f - 22.0f, 10.0f));
		if (!Engine::Game::running) {
			// Button draw
			if (ImGui::Button("PLAY")) {
				// Saving entites
				savedEntities.resize(Engine::Game::entities.size());
				for (int i = 0; i < Engine::Game::entities.size(); i++) {
					savedEntities[i] = *Engine::Game::entities[i];
				}

				// Reloading scripts
				Engine::Scripting::handler->reloadScripts();

				// Calling start scripts
				Engine::Scripting::CallStarts();

				// Other
				Engine::Time::timer.reset();
				Engine::Game::running = true;
			}
		}
		else {
			// Button draw
			if (ImGui::Button("STOP")) {
				// Loading saved entities
				Engine::Game::entities.clear();
				for (Engine::Game::Entity& ent : savedEntities) {
					Engine::Game::entities.newInst(new Engine::Game::Entity(ent));
				}
				savedEntities.clear();

				// Picking
				Engine::Picking::selected = nullptr;
				Engine::Picking::selectedInd = -1;

				// Other
				Engine::Game::running = false;
			}
		}

		// Button focus fix
		Engine::GUI::viewportFocus = Engine::GUI::viewportFocus && !ImGui::IsItemHovered();

		// Saving viewport props
		ImVec2 viewPos = ImGui::GetWindowPos();
		ImVec2 viewSize = ImGui::GetWindowSize();
		Engine::GUI::viewportPos = kl::int2(int(viewPos.x), int(viewPos.y));
		Engine::GUI::viewportSize = kl::int2(int(viewSize.x), int(viewSize.y));

		// End draw
		ImGui::End();
	}
}
