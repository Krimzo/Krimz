#include "Engine/GUI.h"


void Scene() {
	ImGui::SetNextWindowPos(ImVec2(0, mainMenuSize.y - 1));
	ImGui::SetNextWindowSize(ImVec2(winSize.x * sidePanelsWidth, winSize.y - mainMenuSize.y + 1));
	if (ImGui::Begin("Scene")) {
		// Meshes


		// Textures


		// Entities
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		bool noSelection = true;
		for (int i = 0; i < entities.size(); i++) {
			bool state = (selected == entities[i]);
			ImGui::Selectable(entities[i]->name.c_str(), &state);
			if (state) {
				selected = entities[i];
				noSelection = false;
			}
		}
		if (noSelection) {
			selected = nullptr;
		}
		ImGui::PopStyleVar();

		// End draw
		ImGui::End();
	}
}
