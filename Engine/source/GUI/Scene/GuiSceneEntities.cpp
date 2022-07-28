#include "GUI/GUI.h"
#include "Types/Entity.h"
#include "Stage/Stage.h"


static std::string nameBuff = {};
static std::string inputBuff = {};

void Krimz::GUI::Scene::Entites()
{
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Button("New", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
			{
				Krimz::entities.push_back(kl::make<Krimz::Entity>());
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		bool noSelection = true;
		for (int i = 0; auto & ent : Krimz::entities)
		{
			if (ent->getName() == nameBuff)
			{
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuff[0], inputBuff.size(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					ent->updateName(inputBuff.c_str());
					inputBuff.clear();
					nameBuff.clear();
				}
			}
			else
			{
				bool state = (Krimz::Selected::entity == ent);
				ImGui::Selectable(ent->getName().c_str(), &state);
				if (state)
				{
					Krimz::Selected::entity = ent;
					noSelection = false;
				}
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					nameBuff = ent->getName();
					inputBuff.resize(64);
					ImGui::CloseCurrentPopup();
				}
				if (!Krimz::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					if (Krimz::Selected::entity == ent)
					{
						Krimz::Selected::entity = nullptr;
					}
					auto entIt = Krimz::entities.begin();
					std::advance(entIt, i);
					Krimz::entities.erase(entIt);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			i++;
		}
		if (noSelection)
		{
			Krimz::Selected::entity = nullptr;
		}
		ImGui::End();
	}
}
