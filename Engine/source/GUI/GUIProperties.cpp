#include "GUI/GUI.h"
#include "Input/Picking.h"
#include "Scripting/Scripting.h"


void Engine::GUI::Properties() {
	// Geometry
	if (ImGui::Begin("Geometry", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			// Axis
			ImGui::DragFloat3("Scale", (float*)&Engine::Picking::selected->scale, 0.1f);
			ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&Engine::Picking::selected->rotation, 0.1f);

			// Mesh
			if (ImGui::BeginCombo("Mesh", Engine::Picking::selected->mesh->name.c_str())) {
				for (int i = 0; i < Engine::meshes.size(); i++) {
					bool selectedOne = (Engine::Picking::selected->mesh == Engine::meshes[i]);
					if (ImGui::Selectable(Engine::meshes[i]->name.c_str(), selectedOne)) {
						if (selectedOne) {
							ImGui::SetItemDefaultFocus();
						}
						else {
							Engine::Picking::selected->mesh = Engine::meshes[i];
						}
					}
				}

				// End
				ImGui::EndCombo();
			}
		}

		// End draw
		ImGui::End();
	}

	// View
	if (ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			ImGui::Checkbox("Visible", &Engine::Picking::selected->visible);
			ImGui::Checkbox("Shadows", &Engine::Picking::selected->shadows);
			if (ImGui::BeginCombo("Texture", Engine::Picking::selected->texture->name.c_str())) {
				for (int i = 0; i < Engine::textures.size(); i++) {
					bool selectedOne = (Engine::Picking::selected->texture == Engine::textures[i]);
					if (ImGui::Selectable(Engine::textures[i]->name.c_str(), selectedOne)) {
						if (selectedOne) {
							ImGui::SetItemDefaultFocus();
						}
						else {
							Engine::Picking::selected->texture = Engine::textures[i];
						}
					}
				}

				// End
				ImGui::EndCombo();
			}
			ImGui::SliderFloat("Roughness", (float*)&Engine::Picking::selected->roughness, 0.0f, 1.0f);
		}

		// End draw
		ImGui::End();
	}

	// Physics
	if (ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			ImGui::Checkbox("Physics", &Engine::Picking::selected->physics);
			ImGui::DragFloat3("Acceleration", (float*)&Engine::Picking::selected->acceler, 0.1f);
			ImGui::DragFloat3("Veloctiy", (float*)&Engine::Picking::selected->velocity, 0.1f);
			ImGui::DragFloat3("Angular", (float*)&Engine::Picking::selected->angular, 0.1f);
		}

		// End draw
		ImGui::End();
	}

	// Scripts
	if (ImGui::Begin("Script", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			for (int i = 0; i < Engine::Handler::scripts.size(); i++) {
				bool hasScript = [&]() {
					for (int j = 0; j < Engine::Picking::selected->scripts.size(); j++) {
						if (Engine::Picking::selected->scripts[j] == Engine::Handler::scripts[i]) {
							return true;
						}
					}
					return false;
				}();
				const bool lastPhase = hasScript;
				ImGui::Checkbox(Engine::Handler::scripts[i]->name.c_str(), &hasScript);
				if (!lastPhase && hasScript) {
					Engine::Picking::selected->scripts.push_back(Engine::Handler::scripts[i]);
				}
				else if (lastPhase && !hasScript) {
					for (int j = 0; j < Engine::Picking::selected->scripts.size(); j++) {
						if (Engine::Picking::selected->scripts[j] == Engine::Handler::scripts[i]) {
							Engine::Picking::selected->scripts.erase(Engine::Picking::selected->scripts.begin() + j);
							break;
						}
					}
				}
			}
		}

		// End draw
		ImGui::End();
	}
}
