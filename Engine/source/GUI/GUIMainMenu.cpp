#include "GUI/GUI.h"
#include "Utility/Utility.h"
#include "View/Background.h"
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
		if (ImGui::BeginMenu("View")) {
			ImGui::Selectable("Render Colliders", &Engine::Outline::renderColliders);
			ImGui::EndMenu();
		}

		// End draw
		ImGui::EndMainMenuBar();
	}
}
