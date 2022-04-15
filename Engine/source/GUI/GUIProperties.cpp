#include "GUI/GUI.h"
#include "Input/Picking.h"
#include "Scripting/Scripting.h"
#include "Stage/Stage.h"


void PropGeometry()
{
	if (ImGui::Begin("Geometry", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Engine::Picking::selected)
		{
			// Axis
			ImGui::DragFloat3("Scale", (float*)&Engine::Picking::selected->scale, 0.1f);
			ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&Engine::Picking::selected->rotation, 0.1f, 0.0f, 360.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

			// Mesh
			if (ImGui::BeginCombo("Mesh", Engine::Picking::selected->mesh->name.c_str()))
			{
				for (int i = 0; i < Engine::meshes.size(); i++)
				{
					bool selectedOne = (Engine::Picking::selected->mesh == Engine::meshes[i]);
					if (ImGui::Selectable(Engine::meshes[i]->name.c_str(), selectedOne))
					{
						if (selectedOne)
							ImGui::SetItemDefaultFocus();
						else
							Engine::Picking::selected->mesh = Engine::meshes[i];
					}
				}

				// End
				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}
}

void PropView()
{
	if (ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Engine::Picking::selected)
		{
			ImGui::Checkbox("Visible", &Engine::Picking::selected->visible);
			ImGui::Checkbox("Shadows", &Engine::Picking::selected->shadows);
			if (ImGui::BeginCombo("Texture", Engine::Picking::selected->texture->name.c_str()))
			{
				for (int i = 0; i < Engine::textures.size(); i++)
				{
					bool selectedOne = (Engine::Picking::selected->texture == Engine::textures[i]);
					if (ImGui::Selectable(Engine::textures[i]->name.c_str(), selectedOne))
					{
						if (selectedOne)
							ImGui::SetItemDefaultFocus();
						else
							Engine::Picking::selected->texture = Engine::textures[i];
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SliderFloat("Roughness", (float*)&Engine::Picking::selected->roughness, 0.0f, 1.0f);
		}
		ImGui::End();
	}
}

void PropPhysics()
{
	if (ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Engine::Picking::selected)
		{
			ImGui::Checkbox("Physics", &Engine::Picking::selected->physics);
			ImGui::DragFloat3("Acceleration", (float*)&Engine::Picking::selected->acceler, 0.1f);
			ImGui::DragFloat3("Veloctiy", (float*)&Engine::Picking::selected->velocity, 0.1f);
			ImGui::DragFloat3("Angular", (float*)&Engine::Picking::selected->angular, 0.1f);
		}
		ImGui::End();
	}
}

void PropScripts()
{
	if (ImGui::Begin("Scripts", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Engine::Picking::selected)
		{
			// Transfer
			ImVec2 winPos = ImGui::GetWindowPos();
			ImVec2 winSize = ImGui::GetWindowSize();
			ImGuiID winId = ImGui::GetID("Scripts");
			if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId))
			{
				// Highlight
				if (ImGui::GetDragDropPayload()->IsDataType("ScriptTransfer"))
					ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));

				// Payload accept
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ScriptTransfer"))
				{
					std::filesystem::path filePath((char*)payload->Data);
					Engine::Picking::selected->scripts.push_back(Engine::Script(filePath.string()));
				}
				ImGui::EndDragDropTarget();
			}

			// Script names
			for (int i = 0; i < Engine::Picking::selected->scripts.size(); i++)
			{
				// Draw
				ImGui::PushID(i);
				ImGui::Selectable(std::filesystem::path(Engine::Picking::selected->scripts[i].path).stem().string().c_str());
				ImGui::PopID();

				// RMB menu
				if (ImGui::BeginPopupContextItem())
				{
					// Delete
					if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
					{
						Engine::Picking::selected->scripts.erase(Engine::Picking::selected->scripts.begin() + i);
						ImGui::CloseCurrentPopup();
					}

					// End
					ImGui::EndPopup();
				}
			}
		}

		// Script reload
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::Button("Reload", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
			{
				Engine::JavaHandler::ReloadScripts();
				ImGui::CloseCurrentPopup();
			}

			// End
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}

void Engine::GUI::Properties()
{
	PropGeometry();
	PropView();
	PropPhysics();
	PropScripts();
}
