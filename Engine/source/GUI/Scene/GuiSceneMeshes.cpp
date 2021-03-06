#include "GUI/GUI.h"
#include "Types/Mesh.h"
#include "Types/Entity.h"
#include "Stage/Stage.h"


static std::string nameBuffer = {};
static std::string inputBuffer = {};

void Engine::GUI::Scene::Meshes() {
	if (ImGui::Begin("Meshes", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Meshes");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
			if (ImGui::GetDragDropPayload()->IsDataType("MeshTransfer")) {
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
			}
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshTransfer")) {
				std::filesystem::path filePath((char*)payload->Data);
				Engine::meshes.push_back(kl::make<Engine::Mesh>(filePath.stem().string(), kl::file::parseMesh(filePath.string())));
			}
			ImGui::EndDragDropTarget();
		}

		for (int i = 0; auto & mes : Engine::meshes) {
			if (nameBuffer == mes->getName()) {
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuffer[0], inputBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
					mes->updateName(inputBuffer.c_str());
					inputBuffer.clear();
					nameBuffer.clear();
				}
			}
			else {
				ImGui::Selectable(mes->getName().c_str());
			}

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameBuffer = mes->getName();
					inputBuffer.resize(64);
					ImGui::CloseCurrentPopup();
				}
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					for (auto& ent : Engine::entities) {
						if (ent->mesh == mes) {
							ent->mesh = Engine::Meshes::Default::cube;
						}
					}
					auto mesIt = Engine::meshes.begin();
					std::advance(mesIt, i);
					Engine::meshes.erase(mesIt);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			i++;
		}
		ImGui::End();
	}
}
