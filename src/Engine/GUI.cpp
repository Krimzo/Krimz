#include "Engine/Engine.h"

#include "ImGui/imgui.h"


// Panel sizes
const float sidePanelsWidth = 0.2f;
const float explorerHeight = 0.2f;

// Size buffers
kl::ivec2 winSize;
ImVec2 mainMenuSize;

// Maine menu bar
void MainMenu() {
	if (ImGui::BeginMainMenuBar()) {
		// File
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				win.stop();
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
				for (int i = 0; i < skyboxes.size(); i++) {
					bool state = (skybox == skyboxes[i]);
					ImGui::Selectable(skyboxes[i]->name.c_str(), &state);
					if (state) {
						skybox = skyboxes[i];
						noSelection = false;
					}
				}
				if (noSelection) {
					skybox = nullptr;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		// Getting the menu size
		mainMenuSize = ImGui::GetWindowSize();

		// End draw
		ImGui::EndMainMenuBar();
	}
}

// Scene entities
void Scene() {
	ImGui::SetNextWindowPos(ImVec2(0, mainMenuSize.y - 1));
	ImGui::SetNextWindowSize(ImVec2(winSize.x * sidePanelsWidth, winSize.y - mainMenuSize.y + 1));
	if (ImGui::Begin("Scene")) {
		// Meshes


		// Textures


		// Entities
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		bool noSelection = true;
		for (int i = 0; i < entities.size(); i++) {
			bool state = (selected == entities[i]);
			ImGui::Selectable(entities[i]->name.c_str(), &state);
			if (state) {
				selected = entities[i];
				noSelection = false;
			}
		}
		if (noSelection) {
			selected = nullptr;
		}
		ImGui::PopStyleVar();

		// End draw
		ImGui::End();
	}
}

// File explorer
void Explorer() {
	if (ImGui::Begin("Explorer")) {
		// Updating the pos/size
		ImGui::SetWindowPos("Explorer", ImVec2(winSize.x * sidePanelsWidth - 1, winSize.y * (1 - explorerHeight)));
		ImGui::SetWindowSize("Explorer", ImVec2(winSize.x * (1 - 2 * sidePanelsWidth) + 1, winSize.y * explorerHeight));

		// End draw
		ImGui::End();
	}
	else {
		// Updating the pos
		ImGui::SetWindowPos("Explorer", ImVec2(winSize.x * sidePanelsWidth - 1, mainMenuSize.y - 1));
	}
}

// Entity properties panel
void Properties() {
	ImGui::SetNextWindowPos(ImVec2(winSize.x * (1 - sidePanelsWidth) - 1, mainMenuSize.y - 1));
	ImGui::SetNextWindowSize(ImVec2(winSize.x * sidePanelsWidth + 1, winSize.y - mainMenuSize.y + 1));
	if (ImGui::Begin("Entity Properties")) {
		if (selected) {
			ImGui::Checkbox("Visible", &selected->visible);
			ImGui::Checkbox("Shadows", &selected->shadows);
			ImGui::DragFloat3("Size", (float*)&selected->size, 0.1f);
			ImGui::DragFloat3("Position", (float*)&selected->position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&selected->rotation, 0.1f);
			ImGui::Checkbox("Physics", &selected->physics);
			ImGui::DragFloat3("Acceleration", (float*)&selected->acceler, 0.1f);
			ImGui::DragFloat3("Veloctiy", (float*)&selected->velocity, 0.1f);
			ImGui::DragFloat3("Angular", (float*)&selected->angular, 0.1f);
		}

		// End draw
		ImGui::End();
	}
}

// GUI draw function
void GUI() {
	// Getting the win size
	winSize = win.getSize();

	// Main menu
	MainMenu();

	// Scene
	Scene();

	// Explorer
	Explorer();

	// Properties
	Properties();
}
