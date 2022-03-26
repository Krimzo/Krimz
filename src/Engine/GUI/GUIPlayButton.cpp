#include "Engine/GUI/GUIStage.h"
#include "Engine/GUI/GUIProperties.h"
#include "Engine/Game/Entity.h"
#include "Engine/Game/Game.h"
#include "Engine/Input/Picking.h"
#include "Engine/Time/Time.h"
#include "Engine/Scripting/Scripting.h"


std::vector<Engine::Game::Entity> savedEntities;

void Engine::GUI::ViewportOverlay() {
	// Saving old
	ImGuiStyle& style = ImGui::GetStyle();
	const ImVec4 oldColor = style.Colors[ImGuiCol_WindowBg];

	// Transparent setting
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	// Window draw
	ImGui::SetNextWindowPos(ImVec2(float(Engine::GUI::viewportPos.x), float(Engine::GUI::viewportPos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(Engine::GUI::viewportSize.x), float(Engine::GUI::viewportSize.y)));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("Viewport Overlay", nullptr, Engine::GUI::panelFlags | ImGuiWindowFlags_NoTitleBar)) {
		// Focuse save
		Engine::GUI::viewportFocus = ImGui::IsWindowHovered();
		
		// Play button
		ImGui::SetCursorPos(ImVec2(Engine::GUI::explorSize.x * 0.5f - 22.0f, 10.0f));
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

		// End draw
		ImGui::End();
	}
	ImGui::PopStyleVar();

	// Old color reset
	style.Colors[ImGuiCol_WindowBg] = oldColor;
}
