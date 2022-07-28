#include "GUI/GUI.h"
#include "Types/Texture.h"
#include "Types/Entity.h"
#include "Stage/Stage.h"


static std::string nameBuffer = {};
static std::string inputBuffer = {};

void Krimz::GUI::Scene::Textures()
{
	if (ImGui::Begin("Textures", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Textures");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId))
		{
			if (ImGui::GetDragDropPayload()->IsDataType("TextureTransfer"))
			{
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
			}
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer"))
			{
				std::filesystem::path filePath((char*) payload->Data);
				Krimz::textures.push_back(kl::make<Krimz::Texture>(filePath.stem().string(), kl::image(filePath.string())));
			}
			ImGui::EndDragDropTarget();
		}

		for (int i = 0; auto & tex : Krimz::textures)
		{
			if (nameBuffer == tex->getName())
			{
				ImGui::SetKeyboardFocusHere();
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				if (ImGui::InputText("##NewName", &inputBuffer[0], inputBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					tex->updateName(inputBuffer.c_str());
					inputBuffer.clear();
					nameBuffer.clear();
				}
			}
			else
			{
				ImGui::Selectable(tex->getName().c_str());
			}

			if (ImGui::BeginPopupContextItem())
			{
				const float winWidth = ImGui::GetWindowContentRegionWidth();
				ImGui::Image(tex->view, ImVec2(winWidth, winWidth), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				if (ImGui::Button("Rename", ImVec2(winWidth, 0.0f)))
				{
					nameBuffer = tex->getName();
					inputBuffer.resize(64);
					ImGui::CloseCurrentPopup();
				}

				if (!Krimz::gameRunning && ImGui::Button("Delete", ImVec2(winWidth, 0.0f)))
				{
					for (auto& ent : Krimz::entities)
					{
						if (ent->material.colorMap == tex)
						{
							ent->material.colorMap = Krimz::Textures::Default::colorMap;
						}
						if (ent->material.normalMap == tex)
						{
							ent->material.normalMap = Krimz::Textures::Default::nullMap;
						}
						if (ent->material.roughnessMap == tex)
						{
							ent->material.roughnessMap = Krimz::Textures::Default::nullMap;
						}
					}
					auto texIt = Krimz::textures.begin();
					std::advance(texIt, i);
					Krimz::textures.erase(texIt);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			i++;
		}

		static bool displayColorPicker = false;
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::Button("New", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
			{
				displayColorPicker = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		static kl::float3 textureColor;
		if (displayColorPicker)
		{
			if (ImGui::Begin("Texture color"))
			{
				ImGui::ColorPicker3("##Color picker", (float*) &textureColor);
				if (ImGui::Button("Done", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					Krimz::textures.push_back(kl::make<Krimz::Texture>("undefined", kl::image(1, textureColor)));
					displayColorPicker = false;
				}
				ImGui::End();
			}
		}
		ImGui::End();
	}
}
