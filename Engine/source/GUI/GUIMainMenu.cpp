#include "GUI/GUI.h"
#include "Render/Render.h"


void Engine::GUI::MainMenuRender() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				Engine::window.stop();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Render")) {
			ImGui::Selectable("Colliders", &Engine::Colliders::shouldRender);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
