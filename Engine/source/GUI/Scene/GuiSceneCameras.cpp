#include "GUI/GUI.h"


static std::string nameBuffer = {};
static std::string inputBuffer = {};

void Engine::GUI::Scene::Cameras() {
	if (ImGui::Begin("Cameras", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		ImGui::End();
	}
}
