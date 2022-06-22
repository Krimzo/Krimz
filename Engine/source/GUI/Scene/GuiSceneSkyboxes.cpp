#include "GUI/GUI.h"
#include "Types/Skybox.h"
#include "Types/Camera.h"
#include "Stage/Stage.h"


static std::string nameBuffer = {};
static std::string inputBuffer = {};

void Engine::GUI::Scene::Skyboxes() {
	if (ImGui::Begin("Skyboxes", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Skyboxes");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (ImGui::GetDragDropPayload()->IsDataType("TextureTransfer")) {
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
			}
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				std::shared_ptr<Engine::Skybox> loadedSB = std::make_shared<Engine::Skybox>(filePath.filename().stem().string(), kl::image(filePath.string()));
				if (loadedSB->isValid()) {
					Engine::skyboxes.push_back(loadedSB);
				}
			}
			ImGui::EndDragDropTarget();
		}

		bool noSelection = true;
		for (int i = 0; auto & skyb : Engine::skyboxes) {
			if (nameBuffer == skyb->getName()) {
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuffer[0], inputBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
					skyb->updateName(inputBuffer.c_str());
					inputBuffer.clear();
					nameBuffer.clear();
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

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameBuffer = skyb->getName();
					inputBuffer.resize(64);
					ImGui::CloseCurrentPopup();
				}
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

			i++;
		}
		if (noSelection) {
			Engine::Default::camera->skybox = nullptr;
		}
		ImGui::End();
	}
}
