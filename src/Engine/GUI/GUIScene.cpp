#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Window/Window.h"
#include "Engine/Game/Game.h"
#include "Engine/Input/Picking.h"
#include "Engine/Data/Meshes.h"
#include "Engine/Data/Textures.h"
#include "Engine/Scripting/Scripting.h"
#include "Engine/Render/Render.h"
#include "Engine/Logging/Logging.h"
#include "Engine/Game/Game.h"
#include "ImGui/imgui_internal.h"


void Entites() {
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// New entity
		if (ImGui::BeginPopupContextWindow()) {
			// New button
			if (ImGui::Button("New")) {
				Engine::Game::entities.newInst(new Engine::Game::Entity());
				ImGui::CloseCurrentPopup();
			}

			// End
			ImGui::EndPopup();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; i < Engine::Game::entities.size(); i++) {
			// Selectable draw
			bool state = (Engine::Picking::selected == Engine::Game::entities[i]);
			ImGui::Selectable(Engine::Game::entities[i]->name.c_str(), &state);
			if (state) {
				Engine::Picking::selected = Engine::Game::entities[i];
				noSelection = false;
			}

			// RMB menu
			static bool nameInput = false;
			static int nameIndex = 0;
			static char nameBuff[64] = {};
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::Game::entities[i]->name.c_str(),
						Engine::Game::entities[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}

				// Delete
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					if (Engine::Picking::selected == Engine::Game::entities[i]) {
						Engine::Picking::selected = nullptr;
					}
					Engine::Game::entities.delInst(Engine::Game::entities[i]);
				}

				// End
				ImGui::EndPopup();
			}

			// Text input
			if (nameInput && i == nameIndex) {
				if (ImGui::Begin("Text input")) {
					if (ImGui::InputText("New name:", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
						const std::string newName(nameBuff);
						if ([&]() {
							if (newName == Engine::Game::entities[i]->name) {
								nameInput = false;
									return false;
							}
							for (int j = 0; j < Engine::Game::entities.size(); j++) {
								if (Engine::Game::entities[j]->name == newName) {
									return false;
								}
							}
							return true;
						}()) {
							Engine::Game::entities[i]->name = newName;
							nameInput = false;
						}
					}
					ImGui::End();
				}
			}
		}
		if (noSelection) {
			Engine::Picking::selected = nullptr;
		}

		// End child window
		ImGui::End();
	}
}

void Meshes() {
	if (ImGui::Begin("Meshes", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Meshes");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				std::string fileName = filePath.stem().string();
				if ([&](){
					for (int i = 0; i < Engine::Data::meshes.size(); i++) {
						if (Engine::Data::meshes[i]->name == fileName) {
							return false;
						}
					}
					return true;
				}()) {
					Engine::Data::meshes.newInst(new Engine::Mesh(fileName,
						Engine::Render::gpu->newVertBuffer(filePath.string())));
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Mesh names
		for (int i = 0; i < Engine::Data::meshes.size(); i++) {
			// Draw
			ImGui::Selectable(Engine::Data::meshes[i]->name.c_str());

			// RMB menu
			static bool nameInput = false;
			static int nameIndex = 0;
			static char nameBuff[64] = {};
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::Data::meshes[i]->name.c_str(),
						Engine::Data::meshes[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}

				// Delete
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					for (int j = 0; j < Engine::Game::entities.size(); j++) {
						if (Engine::Game::entities[j]->mesh == Engine::Data::meshes[i]) {
							Engine::Game::entities[j]->mesh = Engine::Default::mesh;
						}
					}
					Engine::Data::meshes.delInst(Engine::Data::meshes[i]);
				}

				// End
				ImGui::EndPopup();
			}

			// Text input
			if (nameInput && i == nameIndex) {
				if (ImGui::Begin("Text input")) {
					if (ImGui::InputText("New name: ", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
						const std::string newName(nameBuff);
						if ([&]() {
							if (newName == Engine::Data::meshes[i]->name) {
								nameInput = false;
									return false;
							}
							for (int j = 0; j < Engine::Data::meshes.size(); j++) {
								if (Engine::Data::meshes[j]->name == newName) {
									return false;
								}
							}
							return true;
						}()) {
							Engine::Data::meshes[i]->name = newName;
							nameInput = false;
						}
					}
					ImGui::End();
				}
			}
		}

		// End child window
		ImGui::End();
	}
}

void Textures() {
	if (ImGui::Begin("Textures", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Textures");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				std::string fileName = filePath.stem().string();
				if ([&]() {
					for (int i = 0; i < Engine::Data::textures.size(); i++) {
						if (Engine::Data::textures[i]->name == fileName) {
							return false;
						}
					}
					return true;
				}()) {
					ID3D11Texture2D* tempTex = Engine::Render::gpu->newTexture(kl::image(filePath.string()));
					Engine::Data::textures.newInst(new Engine::Texture(fileName, Engine::Render::gpu->newShaderView(tempTex)));
					Engine::Render::gpu->destroy(tempTex);
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Texture names
		for (int i = 0; i < Engine::Data::textures.size(); i++) {
			// Draw
			ImGui::Selectable(Engine::Data::textures[i]->name.c_str());

			// RMB menu
			static bool nameInput = false;
			static int nameIndex = 0;
			static char nameBuff[64] = {};
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::Data::textures[i]->name.c_str(),
						Engine::Data::textures[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}

				// Delete
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					for (int j = 0; j < Engine::Game::entities.size(); j++) {
						if (Engine::Game::entities[j]->texture == Engine::Data::textures[i]) {
							Engine::Game::entities[j]->texture = Engine::Default::texture;
						}
					}
					Engine::Data::textures.delInst(Engine::Data::textures[i]);
				}

				// End
				ImGui::EndPopup();
			}

			// Text input
			if (nameInput && i == nameIndex) {
				if (ImGui::Begin("Input")) {
					if (ImGui::InputText("New name: ", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
						const std::string newName(nameBuff);
						if ([&]() {
							if (newName == Engine::Data::textures[i]->name) {
								nameInput = false;
									return false;
							}
							for (int j = 0; j < Engine::Data::textures.size(); j++) {
								if (Engine::Data::textures[j]->name == newName) {
									return false;
								}
							}
							return true;
						}()) {
							Engine::Data::textures[i]->name = newName;
							nameInput = false;
						}
					}
					ImGui::End();
				}
			}
		}

		// End child window
		ImGui::End();
	}
}

void Scripts() {
	if (ImGui::Begin("Scripts", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Scripts");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ScriptTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				const std::string className = filePath.stem().string();
				if ([&]() {
					for (int i = 0; i < Engine::Scripting::handler->scripts.size(); i++) {
						if (Engine::Scripting::handler->scripts[i]->getName() == className) {
							return false;
						}
					}
					return true;
				}()) {
					Engine::Scripting::handler->newScript(className, filePath.string());
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Script names
		for (int i = 0; i < Engine::Scripting::handler->scripts.size(); i++) {
			// Draw
			ImGui::Selectable(Engine::Scripting::handler->scripts[i]->getName().c_str());

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Delete
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					for (int j = 0; j < Engine::Game::entities.size(); j++) {
						for (int k = 0; k < Engine::Game::entities[j]->scripts.size(); k++) {
							if (Engine::Game::entities[j]->scripts[k] == Engine::Scripting::handler->scripts[i]) {
								Engine::Game::entities[j]->scripts.erase(Engine::Game::entities[j]->scripts.begin() + k);
							}
						}
					}
					Engine::Scripting::handler->delScript(Engine::Scripting::handler->scripts[i]);
					Engine::Scripting::handler->reloadScripts();
				}

				// End
				ImGui::EndPopup();
			}
		}

		// End child window
		ImGui::End();
	}
}

void Engine::GUI::Scene() {
	// Allignement
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

	// Draw
	Entites();
	Meshes();
	Textures();
	Scripts();

	// Style pop
	ImGui::PopStyleVar();
}
