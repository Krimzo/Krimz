#include "Engine/GUI.h"


void Scene() {
	guiScenePos = kl::int2(int(0), int(mainMenuSize.y));
	guiSceneSize = kl::int2(int(winSize.x * sidePanelsRatio), int(winSize.y - mainMenuSize.y));
	ImGui::SetNextWindowPos(ImVec2(float(guiScenePos.x), float(guiScenePos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(guiSceneSize.x), float(guiSceneSize.y)));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("Scene", nullptr, panelWinFlags)) {
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
	ImGui::PopStyleVar();
}
