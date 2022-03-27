#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Input/Picking.h"


void Engine::GUI::Properties() {
	// Geometry
	if (ImGui::Begin("Geometry", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			// Axis
			ImGui::DragFloat3("Size", (float*)&Engine::Picking::selected->size, 0.1f);
			ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&Engine::Picking::selected->rotation, 0.1f);

			// Mesh
			if (ImGui::BeginCombo("Mesh", Engine::Picking::selected->mesh->name.c_str())) {
				for (int i = 0; i < Engine::Data::meshes.size(); i++) {
					bool selectedOne = (Engine::Picking::selected->mesh == Engine::Data::meshes[i]);
					if (ImGui::Selectable(Engine::Data::meshes[i]->name.c_str(), selectedOne)) {
						if (selectedOne) {
							ImGui::SetItemDefaultFocus();
						}
						else {
							Engine::Picking::selected->mesh = Engine::Data::meshes[i];
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
				for (int i = 0; i < Engine::Data::textures.size(); i++) {
					bool selectedOne = (Engine::Picking::selected->texture == Engine::Data::textures[i]);
					if (ImGui::Selectable(Engine::Data::textures[i]->name.c_str(), selectedOne)) {
						if (selectedOne) {
							ImGui::SetItemDefaultFocus();
						}
						else {
							Engine::Picking::selected->texture = Engine::Data::textures[i];
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
			
		}

		// End draw
		ImGui::End();
	}
}
