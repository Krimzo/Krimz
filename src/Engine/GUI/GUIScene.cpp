#include "Engine/GUI/GUI.h"


void Engine::GUI::Scene() {
	// Window pos/size calc
	Engine::GUI::scenePos = kl::int2(int(0), int(Engine::GUI::mMenuSize.y));
	Engine::GUI::sceneSize = kl::int2(
		int(Engine::Window::size.x * Engine::GUI::panelRatio),
		int(Engine::Window::size.y - Engine::GUI::mMenuSize.y)
	);

	// Window pos/size set
	ImGui::SetNextWindowPos(ImVec2(float(Engine::GUI::scenePos.x), float(Engine::GUI::scenePos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(Engine::GUI::sceneSize.x), float(Engine::GUI::sceneSize.y)));

	// Window draw
	if (ImGui::Begin("Scene", nullptr, Engine::GUI::panelFlags)) {
		// Meshes


		// Textures


		// Entities
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
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
		ImGui::PopStyleVar();

		// End draw
		ImGui::End();
	}
}
