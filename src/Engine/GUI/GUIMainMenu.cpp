#include "Engine/GUI/GUI.h"


void Engine::GUI::MainMenu() {
	if (ImGui::BeginMainMenuBar()) {
		// File
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				Engine::Window::win.stop();
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
				for (int i = 0; i < Engine::Background::skyboxes.size(); i++) {
					bool state = (Engine::Background::skybox == Engine::Background::skyboxes[i]);
					ImGui::Selectable(Engine::Background::skyboxes[i]->name.c_str(), &state);
					if (state) {
						Engine::Background::skybox = Engine::Background::skyboxes[i];
						noSelection = false;
					}
				}
				if (noSelection) {
					Engine::Background::skybox = nullptr;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		// Getting the menu size
		Engine::GUI::mMenuSize = ImGui::GetWindowSize();

		// End draw
		ImGui::EndMainMenuBar();
	}
}
