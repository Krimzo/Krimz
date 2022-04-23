#include "GUI/GUI.h"
#include "Utility/Utility.h"
#include "Input/Picking.h"
#include "Data/Meshes.h"
#include "Data/Textures.h"
#include "Data/Entities.h"
#include "Data/Skyboxes.h"
#include "Stage/Stage.h"
#include "Scripting/Scripting.h"
#include "Render/Render.h"
#include "Logging/Logging.h"
#include "View/Background.h"


void Entites() {
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// New entity
		if (ImGui::BeginPopupContextWindow()) {
			// New button
			if (ImGui::Button("New", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
				Engine::entities.push_back(Engine::Entity());
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; auto & ent : Engine::entities) {
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex) {
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					const std::string newName(nameBuff);
					if (newName == ent.name) {
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::entities, newName)) {
						ent.name = newName;
						nameIndex = -1;
					}
				}
			}
			// Draw
			else {
				bool state = (Engine::Picking::selected == &ent);
				ImGui::Selectable(ent.name.c_str(), &state);
				if (state) {
					Engine::Picking::selected = &ent;
					noSelection = false;
				}
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameIndex = i;
					memcpy(nameBuff, ent.name.c_str(), ent.name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					if (Engine::Picking::selected == &ent) {
						Engine::Picking::selected = nullptr;
					}
					auto entIt = Engine::entities.begin();
					std::advance(entIt, i);
					Engine::entities.erase(entIt);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			// Index
			i++;
		}
		if (noSelection) {
			Engine::Picking::selected = nullptr;
		}
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
				Engine::meshes.push_back(Engine::Mesh(filePath.stem().string(), kl::file::parseObj(filePath.string())));
			}
			ImGui::EndDragDropTarget();
		}

		// Mesh names
		for (int i = 0; auto & mes : Engine::meshes) {
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex) {
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					const std::string newName(nameBuff);
					if (newName == mes.name) {
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::meshes, newName)) {
						mes.name = newName;
						nameIndex = -1;
					}
				}
			}
			else
				ImGui::Selectable(mes.name.c_str());

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameIndex = i;
					memcpy(nameBuff, mes.name.c_str(), mes.name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					for (auto& ent : Engine::entities) {
						if (ent.mesh == &mes) {
							ent.mesh = Engine::Default::cube;
						}
					}
					auto mesIt = Engine::meshes.begin();
					std::advance(mesIt, i);
					Engine::meshes.erase(mesIt);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			// Index
			i++;
		}
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
				Engine::textures.push_back(Engine::Texture(filePath.stem().string(), kl::image(filePath.string())));
			}
			ImGui::EndDragDropTarget();
		}

		// Texture names
		for (int i = 0; auto & tex : Engine::textures) {
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex) {
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					const std::string newName(nameBuff);
					if (newName == tex.name) {
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::textures, newName)) {
						tex.name = newName;
						nameIndex = -1;
					}
				}
			}
			else {
				ImGui::Selectable(tex.name.c_str());
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Popup window width
				const float winWidth = ImGui::GetWindowContentRegionWidth();

				// Texture display
				ImGui::Image(tex.view, ImVec2(winWidth, winWidth), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				// Rename
				if (ImGui::Button("Rename", ImVec2(winWidth, 0.0f))) {
					nameIndex = i;
					memcpy(nameBuff, tex.name.c_str(), tex.name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(winWidth, 0.0f))) {
					for (auto& ent : Engine::entities) {
						if (ent.texture == &tex) {
							ent.texture = Engine::Default::texture;
						}
					}
					auto texIt = Engine::textures.begin();
					std::advance(texIt, i);
					Engine::textures.erase(texIt);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			// Index
			i++;
		}

		// New texture popup
		static bool displayColorPicker = false;
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::Button("New", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
				displayColorPicker = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Color picker
		static kl::float3 textureColor;
		if (displayColorPicker) {
			if (ImGui::Begin("Texture color")) {
				ImGui::ColorPicker3("##Color picker", (float*)&textureColor);
				if (ImGui::Button("Done", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					Engine::textures.push_back(Engine::Texture("undefined", kl::image(kl::int2(1), kl::convert::toColor(textureColor))));
					displayColorPicker = false;
				}
				ImGui::End();
			}
		}
		ImGui::End();
	}
}

void Skyboxes() {
	if (ImGui::Begin("Skyboxes", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Skyboxes");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			// Highlight
			if (ImGui::GetDragDropPayload()->IsDataType("TextureTransfer")) {
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
			}

			// Payload accept
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				Engine::Skybox loadedSB = Engine::Skybox(filePath.filename().stem().string(), kl::image(filePath.string()));
				if (loadedSB.isValid()) {
					Engine::skyboxes.push_back(loadedSB);
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; auto & skyb : Engine::skyboxes) {
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex) {
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					const std::string newName(nameBuff);
					if (newName == skyb.name) {
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::skyboxes, newName)) {
						skyb.name = newName;
						nameIndex = -1;
					}
				}
			}
			else {
				bool state = (Engine::Background::skybox == &skyb);
				ImGui::Selectable(skyb.name.c_str(), &state);
				if (state) {
					Engine::Background::skybox = &skyb;
					noSelection = false;
				}
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameIndex = i;
					memcpy(nameBuff, skyb.name.c_str(), skyb.name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					if (Engine::Background::skybox == &skyb) {
						Engine::Background::skybox = nullptr;
					}
					auto skybIt = Engine::skyboxes.begin();
					std::advance(skybIt, i);
					Engine::skyboxes.erase(skybIt);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			// Index
			i++;
		}
		if (noSelection) {
			Engine::Background::skybox = nullptr;
		}
		ImGui::End();
	}
}

void Engine::GUI::Scene() {
	Entites();
	Meshes();
	Skyboxes();
	Textures();
}
