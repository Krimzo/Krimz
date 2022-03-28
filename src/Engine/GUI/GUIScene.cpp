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


void Engine::GUI::Scene() {
	// Allignement
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

	// Entities
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
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
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::Game::entities[i]->name.c_str(),
						Engine::Game::entities[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					if (Engine::Picking::selected == Engine::Game::entities[i]) {
						Engine::Picking::selected = nullptr;
					}
					Engine::Game::entities.delInst(Engine::Game::entities[i]);
				}
				ImGui::EndPopup();
			}

			// Text input
			if (nameInput && i == nameIndex) {
				if (ImGui::Begin("Text input")) {
					if (ImGui::InputText("New name: ", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
						Engine::Game::entities[i]->name = std::string(nameBuff);
						nameInput = false;
					}
					ImGui::End();
				}
			}
		}
		if (noSelection) {
			Engine::Picking::selected = nullptr;
		}

		// New entity
		if (ImGui::Selectable("+")) {
			Engine::Game::entities.newInst(new Engine::Game::Entity());
		}

		// End child window
		ImGui::End();
	}

	// Meshes
	if (ImGui::Begin("Meshes", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Meshes");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				Engine::Data::meshes.newInst(new Engine::Mesh(filePath.stem().string(),
					Engine::Render::gpu->newVertBuffer(filePath.string())));
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
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::Data::meshes[i]->name.c_str(),
						Engine::Data::meshes[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					for (int j = 0; j < Engine::Game::entities.size(); j++) {
						if (Engine::Game::entities[j]->mesh == Engine::Data::meshes[i]) {
							Engine::Game::entities[j]->mesh = Engine::Default::mesh;
						}
					}
					Engine::Data::meshes.delInst(Engine::Data::meshes[i]);
				}
				ImGui::EndPopup();
			}

			// Text input
			if (nameInput && i == nameIndex) {
				if (ImGui::Begin("Text input")) {
					if (ImGui::InputText("New name: ", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
						Engine::Data::meshes[i]->name = std::string(nameBuff);
						nameInput = false;
					}
					ImGui::End();
				}
			}
		}

		// End child window
		ImGui::End();
	}

	// Textures
	if (ImGui::Begin("Textures", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Textures");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				Engine::Data::textures.newInst(new Engine::Texture(filePath.stem().string(),
					Engine::Render::gpu->newShaderView(Engine::Render::gpu->newTexture(kl::image(filePath.string())))));
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
				if (ImGui::Button("Rename")) {
					memcpy(nameBuff,
						Engine::Data::textures[i]->name.c_str(),
						Engine::Data::textures[i]->name.size() + 1);
					nameInput = true;
					nameIndex = i;
				}
				if (!Engine::Game::running && ImGui::Button("Delete")) {
					for (int j = 0; j < Engine::Game::entities.size(); j++) {
						if (Engine::Game::entities[j]->texture == Engine::Data::textures[i]) {
							Engine::Game::entities[j]->texture = Engine::Default::texture;
						}
					}
					Engine::Data::textures.delInst(Engine::Data::textures[i]);
				}
				ImGui::EndPopup();
			}

			// Text input
			if (nameInput && i == nameIndex) {
				if (ImGui::Begin("Text input")) {
					if (ImGui::InputText("New name: ", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
						Engine::Data::textures[i]->name = std::string(nameBuff);
						nameInput = false;
					}
					ImGui::End();
				}
			}
		}

		// End child window
		ImGui::End();
	}

	// Scripts
	if (ImGui::Begin("Scripts", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Scripts");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ScriptTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				const std::string className = filePath.stem().string();
				bool canLoad = true;
				for (int i = 0; i < Engine::Scripting::handler->scripts.size(); i++) {
					if (Engine::Scripting::handler->scripts[i]->getName() == className) {
						canLoad = false;
						break;
					}
				}
				if (canLoad) {
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
				ImGui::EndPopup();
			}
		}

		// End child window
		ImGui::End();
	}

	// Style pop
	ImGui::PopStyleVar();
}
