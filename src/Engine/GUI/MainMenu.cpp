#include "Engine/GUI.h"


void MainMenu() {
	if (ImGui::BeginMainMenuBar()) {
		// File
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				win.stop();
			}
			ImGui::EndMenu();
		}

		// Edit
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("WIP")) {

			}
			ImGui::EndMenu();
		}

		// View
		if (ImGui::BeginMenu("View")) {
			if (ImGui::BeginMenu("Skybox")) {
				bool noSelection = true;
				for (int i = 0; i < skyboxes.size(); i++) {
					bool state = (skybox == skyboxes[i]);
					ImGui::Selectable(skyboxes[i]->name.c_str(), &state);
					if (state) {
						skybox = skyboxes[i];
						noSelection = false;
					}
				}
				if (noSelection) {
					skybox = nullptr;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		// Getting the menu size
		mainMenuSize = ImGui::GetWindowSize();

		// End draw
		ImGui::EndMainMenuBar();
	}
}
