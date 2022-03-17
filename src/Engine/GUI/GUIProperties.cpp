#include "Engine/GUI/GUI.h"


void Engine::GUI::Properties() {
	// Window pos/size calc
	Engine::GUI::propsPos = kl::int2(Engine::GUI::sceneSize.x + Engine::GUI::explorSize.x, Engine::GUI::scenePos.y);
	Engine::GUI::propsSize = Engine::GUI::sceneSize;

	// Window pos/size set
	ImGui::SetNextWindowPos(ImVec2(float(Engine::GUI::propsPos.x), float(Engine::GUI::propsPos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(Engine::GUI::propsSize.x), float(Engine::GUI::propsSize.y)));

	// Window style push
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

	// Window draw
	if (ImGui::Begin("Properties", nullptr, Engine::GUI::panelFlags)) {
		if (Engine::Picking::selected) {
			ImGui::Checkbox("Visible", &Engine::Picking::selected->visible);
			ImGui::Checkbox("Shadows", &Engine::Picking::selected->shadows);
			ImGui::DragFloat3("Size", (float*)&Engine::Picking::selected->size, 0.1f);
			ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&Engine::Picking::selected->rotation, 0.1f);
			ImGui::SliderFloat("Roughness", (float*)&Engine::Picking::selected->roughness, 0.0f, 1.0f);
			ImGui::Checkbox("Physics", &Engine::Picking::selected->physics);
			ImGui::DragFloat3("Acceleration", (float*)&Engine::Picking::selected->acceler, 0.1f);
			ImGui::DragFloat3("Veloctiy", (float*)&Engine::Picking::selected->velocity, 0.1f);
			ImGui::DragFloat3("Angular", (float*)&Engine::Picking::selected->angular, 0.1f);
		}

		// End draw
		ImGui::End();
	}

	// Window style pop
	ImGui::PopStyleVar();
}
