#include "Engine/GUI.h"


void Properties() {
	guiPropsPos = kl::int2(guiSceneSize.x + guiExplorSize.x, guiScenePos.y);
	guiPropsSize = guiSceneSize;
	ImGui::SetNextWindowPos(ImVec2(float(guiPropsPos.x), float(guiPropsPos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(guiPropsSize.x), float(guiPropsSize.y)));
	if (ImGui::Begin("Properties", nullptr, panelWinFlags)) {
		if (selected) {
			ImGui::Checkbox("Visible", &selected->visible);
			ImGui::Checkbox("Shadows", &selected->shadows);
			ImGui::DragFloat3("Size", (float*)&selected->size, 0.1f);
			ImGui::DragFloat3("Position", (float*)&selected->position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&selected->rotation, 0.1f);
			ImGui::SliderFloat("Roughness", (float*)&selected->roughness, 0.0f, 1.0f);
			ImGui::Checkbox("Physics", &selected->physics);
			ImGui::DragFloat3("Acceleration", (float*)&selected->acceler, 0.1f);
			ImGui::DragFloat3("Veloctiy", (float*)&selected->velocity, 0.1f);
			ImGui::DragFloat3("Angular", (float*)&selected->angular, 0.1f);
		}

		// End draw
		ImGui::End();
	}
}
