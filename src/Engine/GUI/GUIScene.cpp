#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Window/Window.h"
#include "Engine/Game/Game.h"
#include "Engine/Input/Picking.h"


void Engine::GUI::Scene() {
	// Allignement
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

	// Entities
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Draw
		bool noSelection = true;
		for (int i = 0; i < Engine::Game::entities.size(); i++) {
			bool state = (Engine::Picking::selected == Engine::Game::entities[i]);
			ImGui::Selectable(Engine::Game::entities[i]->name.c_str(), &state);
			if (state) {
				Engine::Picking::selected = Engine::Game::entities[i];
				noSelection = false;
			}
		}
		if (noSelection) {
			Engine::Picking::selected = nullptr;
		}

		// End child window
		ImGui::End();
	}

	// Textures
	if (ImGui::Begin("Textures", nullptr, ImGuiWindowFlags_NoScrollbar)) {


		// End child window
		ImGui::End();
	}

	// Meshes
	if (ImGui::Begin("Meshes", nullptr, ImGuiWindowFlags_NoScrollbar)) {


		// End child window
		ImGui::End();
	}

	// Style pop
	ImGui::PopStyleVar();
}
