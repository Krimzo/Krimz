#include "GUI/GUI.h"
#include "Render/Render.h"


void Engine::GUI::MainMenu() {
	if (ImGui::BeginMainMenuBar()) {
		// File
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				Engine::win.stop();
			}
			ImGui::EndMenu();
		}

		// View
		if (ImGui::BeginMenu("Render")) {
			ImGui::Selectable("Colliders", &Engine::Outline::renderColliders);
			ImGui::EndMenu();
		}

		// End draw
		ImGui::EndMainMenuBar();
	}
}
