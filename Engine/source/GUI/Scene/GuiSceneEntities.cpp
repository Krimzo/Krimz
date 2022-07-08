#include "GUI/GUI.h"
#include "Types/Entity.h"
#include "Stage/Stage.h"


static std::string nameBuff = {};
static std::string inputBuff = {};

void Engine::GUI::Scene::Entites() {
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Button("New", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
				Engine::entities.push_back(kl::make<Engine::Entity>());
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		bool noSelection = true;
		for (int i = 0; auto & ent : Engine::entities) {
			if (ent->getName() == nameBuff) {
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuff[0], inputBuff.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
					ent->updateName(inputBuff.c_str());
					inputBuff.clear();
					nameBuff.clear();
				}
			}
			else {
				bool state = (Engine::Selected::entity == ent);
				ImGui::Selectable(ent->getName().c_str(), &state);
				if (state) {
					Engine::Selected::entity = ent;
					noSelection = false;
				}
			}

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					nameBuff = ent->getName();
					inputBuff.resize(64);
					ImGui::CloseCurrentPopup();
				}
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

			i++;
		}
		if (noSelection) {
			Engine::Selected::entity = nullptr;
		}
		ImGui::End();
	}
}
