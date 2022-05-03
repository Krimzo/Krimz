#include "GUI/GUI.h"
#include "Editor/Picking.h"
#include "Data/Meshes.h"
#include "Data/Textures.h"
#include "Data/Entities.h"
#include "Data/Skyboxes.h"
#include "Stage/Stage.h"
#include "Scripting/Scripting.h"
#include "Render/Render.h"
#include "Logging/Logging.h"


void Cameras() {
	static String nameBuff = {};
	static String inputBuff = {};

	if (ImGui::Begin("Cameras", nullptr, ImGuiWindowFlags_NoScrollbar)) {

		ImGui::End();
	}
}

void Entites() {
	static String nameBuff = {};
	static String inputBuff = {};

	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// New entity
		if (ImGui::BeginPopupContextWindow()) {
			// New button
			if (ImGui::Button("New", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
				Engine::entities.push_back(std::make_shared<Engine::Entity>());
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; auto & ent : Engine::entities) {
			// Name input
			if (ent->getName() == nameBuff) {
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuff[0], inputBuff.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
					ent->updateName(inputBuff.c_str());
					inputBuff.clear();
					nameBuff.clear();
				}
			}
			// Draw
			else {
				bool state = (Engine::Selected::entity == ent);
				ImGui::Selectable(ent->getName().c_str(), &state);
				if (state) {
					Engine::Selected::entity = ent;
					noSelection = false;
				}
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameBuff = ent->getName();
					inputBuff.resize(64);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					if (Engine::Selected::entity == ent) {
						Engine::Selected::entity = nullptr;
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
			Engine::Selected::entity = nullptr;
		}
		ImGui::End();
	}
}

void Meshes() {
	static String nameBuff = {};
	static String inputBuff = {};

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
				Engine::meshes.push_back(std::make_shared<Engine::Mesh>(filePath.stem().string(), kl::file::parseObj(filePath.string())));
			}
			ImGui::EndDragDropTarget();
		}

		// Mesh names
		for (int i = 0; auto & mes : Engine::meshes) {
			// Name input
			if (nameBuff == mes->getName()) {
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuff[0], inputBuff.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
					mes->updateName(inputBuff.c_str());
					inputBuff.clear();
					nameBuff.clear();
				}
			}
			else {
				ImGui::Selectable(mes->getName().c_str());
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameBuff = mes->getName();
					inputBuff.resize(64);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					for (auto& ent : Engine::entities) {
						if (ent->mesh == mes) {
							ent->mesh = Engine::Default::cube;
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
	static String nameBuff = {};
	static String inputBuff = {};

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
				Engine::textures.push_back(std::make_shared<Engine::Texture>(filePath.stem().string(), kl::image(filePath.string())));
			}
			ImGui::EndDragDropTarget();
		}

		// Texture names
		for (int i = 0; auto & tex : Engine::textures) {
			// Name input
			if (nameBuff == tex->getName()) {
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuff[0], inputBuff.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
					tex->updateName(inputBuff.c_str());
					inputBuff.clear();
					nameBuff.clear();
				}
			}
			else {
				ImGui::Selectable(tex->getName().c_str());
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Popup window width
				const float winWidth = ImGui::GetWindowContentRegionWidth();

				// Texture display
				ImGui::Image(tex->view, ImVec2(winWidth, winWidth), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				// Rename
				if (ImGui::Button("Rename", ImVec2(winWidth, 0.0f))) {
					nameBuff = tex->getName();
					inputBuff.resize(64);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(winWidth, 0.0f))) {
					for (auto& ent : Engine::entities) {
						if (ent->texture == tex) {
							ent->texture = Engine::Default::texture;
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
					Engine::textures.push_back(std::make_shared<Engine::Texture>("undefined", kl::image(kl::int2(1), kl::convert::toColor(textureColor))));
					displayColorPicker = false;
				}
				ImGui::End();
			}
		}
		ImGui::End();
	}
}

void Skyboxes() {
	static String nameBuff = {};
	static String inputBuff = {};

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
				std::shared_ptr<Engine::Skybox> loadedSB = std::make_shared<Engine::Skybox>(filePath.filename().stem().string(), kl::image(filePath.string()));
				if (loadedSB->isValid()) {
					Engine::skyboxes.push_back(loadedSB);
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; auto & skyb : Engine::skyboxes) {
			// Name input
			if (nameBuff == skyb->getName()) {
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuff[0], inputBuff.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
					skyb->updateName(inputBuff.c_str());
					inputBuff.clear();
					nameBuff.clear();
				}
			}
			else {
				bool state = (Engine::Default::camera->skybox == skyb);
				ImGui::Selectable(skyb->getName().c_str(), &state);
				if (state) {
					Engine::Default::camera->skybox = skyb;
					noSelection = false;
				}
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem()) {
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameBuff = skyb->getName();
					inputBuff.resize(64);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					if (Engine::Default::camera->skybox == skyb) {
						Engine::Default::camera->skybox = nullptr;
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
			Engine::Default::camera->skybox = nullptr;
		}
		ImGui::End();
	}
}

void Engine::GUI::Scene() {
	Cameras();
	Entites();
	Meshes();
	Skyboxes();
	Textures();
}
