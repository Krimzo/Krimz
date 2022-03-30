#include "GUI/GUI.h"
#include "Utility/Window.h"
#include "Game/Game.h"
#include "Input/Picking.h"
#include "Render/Meshes.h"
#include "Render/Textures.h"
#include "Scripting/Scripting.h"
#include "Render/Render.h"
#include "Logging/Logging.h"
#include "Game/Game.h"
#include "imgui_internal.h"


void Entites() {
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// New entity
		if (ImGui::BeginPopupContextWindow()) {
			// New button
			if (ImGui::Button("New")) {
				Engine::entities.newInst(new Engine::Entity());
				ImGui::CloseCurrentPopup();
			}

			// End
			ImGui::EndPopup();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; i < Engine::entities.size(); i++) {
			// Selectable draw
			bool state = (Engine::Picking::selected == Engine::entities[i]);
			ImGui::Selectable(Engine::entities[i]->name.c_str(), &state);
			if (state) {
				Engine::Picking::selected = Engine::entities[i];
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
						Engine::entities[i]->name.c_str(),
						Engine::entities[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}

				// Delete
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					if (Engine::Picking::selected == Engine::entities[i]) {
						Engine::Picking::selected = nullptr;
					}
					Engine::entities.delInst(Engine::entities[i]);
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
							if (newName == Engine::entities[i]->name) {
								nameInput = false;
									return false;
							}
							for (int j = 0; j < Engine::entities.size(); j++) {
								if (Engine::entities[j]->name == newName) {
									return false;
								}
							}
							return true;
						}()) {
							Engine::entities[i]->name = newName;
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
			// Highlight
			if (ImGui::GetDragDropPayload()->IsDataType("MeshTransfer")) {
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
			}

			// Payload accept
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				std::string fileName = filePath.stem().string();
				if ([&](){
					for (int i = 0; i < Engine::meshes.size(); i++) {
						if (Engine::meshes[i]->name == fileName) {
							return false;
						}
					}
					return true;
				}()) {
					Engine::meshes.newInst(new Engine::Mesh(fileName,
						Engine::Render::gpu->newVertBuffer(filePath.string())));
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Mesh names
		for (int i = 0; i < Engine::meshes.size(); i++) {
			// Draw
			ImGui::Selectable(Engine::meshes[i]->name.c_str());

			// RMB menu
			static bool nameInput = false;
			static int nameIndex = 0;
			static char nameBuff[64] = {};
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::meshes[i]->name.c_str(),
						Engine::meshes[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}

				// Delete
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					for (int j = 0; j < Engine::entities.size(); j++) {
						if (Engine::entities[j]->mesh == Engine::meshes[i]) {
							Engine::entities[j]->mesh = Engine::Default::mesh;
						}
					}
					Engine::meshes.delInst(Engine::meshes[i]);
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
							if (newName == Engine::meshes[i]->name) {
								nameInput = false;
									return false;
							}
							for (int j = 0; j < Engine::meshes.size(); j++) {
								if (Engine::meshes[j]->name == newName) {
									return false;
								}
							}
							return true;
						}()) {
							Engine::meshes[i]->name = newName;
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
			// Highlight
			if (ImGui::GetDragDropPayload()->IsDataType("TextureTransfer")) {
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
			}

			// Payload accept
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				std::string fileName = filePath.stem().string();
				if ([&]() {
					for (int i = 0; i < Engine::textures.size(); i++) {
						if (Engine::textures[i]->name == fileName) {
							return false;
						}
					}
					return true;
				}()) {
					ID3D11Texture2D* tempTex = Engine::Render::gpu->newTexture(kl::image(filePath.string()));
					Engine::textures.newInst(new Engine::Texture(fileName, Engine::Render::gpu->newShaderView(tempTex)));
					Engine::Render::gpu->destroy(tempTex);
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Texture names
		for (int i = 0; i < Engine::textures.size(); i++) {
			// Draw
			ImGui::Selectable(Engine::textures[i]->name.c_str());

			// RMB menu
			static bool nameInput = false;
			static int nameIndex = 0;
			static char nameBuff[64] = {};
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::textures[i]->name.c_str(),
						Engine::textures[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}

				// Delete
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					for (int j = 0; j < Engine::entities.size(); j++) {
						if (Engine::entities[j]->texture == Engine::textures[i]) {
							Engine::entities[j]->texture = Engine::Default::texture;
						}
					}
					Engine::textures.delInst(Engine::textures[i]);
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
							if (newName == Engine::textures[i]->name) {
								nameInput = false;
									return false;
							}
							for (int j = 0; j < Engine::textures.size(); j++) {
								if (Engine::textures[j]->name == newName) {
									return false;
								}
							}
							return true;
						}()) {
							Engine::textures[i]->name = newName;
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
			// Highlight
			if (ImGui::GetDragDropPayload()->IsDataType("ScriptTransfer")) {
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
			}

			// Payload accept
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
					for (int j = 0; j < Engine::entities.size(); j++) {
						for (int k = 0; k < Engine::entities[j]->scripts.size(); k++) {
							if (Engine::entities[j]->scripts[k] == Engine::Scripting::handler->scripts[i]) {
								Engine::entities[j]->scripts.erase(Engine::entities[j]->scripts.begin() + k);
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
