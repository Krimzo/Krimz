#include "GUI/GUI.h"
#include "Utility/Window.h"
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


void Entites()
{
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		// New entity
		if (ImGui::BeginPopupContextWindow())
		{
			// New button
			if (ImGui::Button("New", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
			{
				Engine::entities.newInst(new Engine::Entity());
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; i < Engine::entities.size(); i++)
		{
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex)
			{
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					const std::string newName(nameBuff);
					if (newName == Engine::entities[i]->name)
					{
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::entities, newName))
					{
						Engine::entities[i]->name = newName;
						nameIndex = -1;
					}
				}
			}
			// Draw
			else
			{
				bool state = (Engine::Picking::selected == Engine::entities[i]);
				ImGui::Selectable(Engine::entities[i]->name.c_str(), &state);
				if (state)
				{
					Engine::Picking::selected = Engine::entities[i];
					noSelection = false;
				}
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem())
			{
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					nameIndex = i;
					memcpy(nameBuff,
						Engine::entities[i]->name.c_str(),
						Engine::entities[i]->name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					if (Engine::Picking::selected == Engine::entities[i])
						Engine::Picking::selected = nullptr;
					Engine::entities.delInst(Engine::entities[i]);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		if (noSelection)
			Engine::Picking::selected = nullptr;
		ImGui::End();
	}
}

void Meshes()
{
	if (ImGui::Begin("Meshes", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Meshes");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId))
		{
			// Highlight
			if (ImGui::GetDragDropPayload()->IsDataType("MeshTransfer"))
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));

			// Payload accept
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshTransfer"))
			{
				std::filesystem::path filePath((char*)payload->Data);
				Engine::meshes.newInst(new Engine::Mesh(filePath.stem().string(),
					Engine::Render::gpu->newVertBuffer(filePath.string())));
			}
			ImGui::EndDragDropTarget();
		}

		// Mesh names
		for (int i = 0; i < Engine::meshes.size(); i++)
		{
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex)
			{
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					const std::string newName(nameBuff);
					if (newName == Engine::meshes[i]->name)
					{
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::meshes, newName))
					{
						Engine::meshes[i]->name = newName;
						nameIndex = -1;
					}
				}
			}
			else
			{
				ImGui::Selectable(Engine::meshes[i]->name.c_str());
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem())
			{
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					nameIndex = i;
					memcpy(nameBuff,
						Engine::meshes[i]->name.c_str(),
						Engine::meshes[i]->name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					for (int j = 0; j < Engine::entities.size(); j++)
					{
						if (Engine::entities[j]->mesh == Engine::meshes[i])
							Engine::entities[j]->mesh = Engine::Default::mesh;
					}
					Engine::meshes.delInst(Engine::meshes[i]);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
}

void Textures()
{
	if (ImGui::Begin("Textures", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Textures");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId))
		{
			// Highlight
			if (ImGui::GetDragDropPayload()->IsDataType("TextureTransfer"))
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));

			// Payload accept
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer"))
			{
				std::filesystem::path filePath((char*)payload->Data);
				ID3D11Texture2D* tempTex = Engine::Render::gpu->newTexture(kl::image(filePath.string()));
				Engine::textures.newInst(new Engine::Texture(filePath.stem().string(),
					Engine::Render::gpu->newShaderView(tempTex)));
				Engine::Render::gpu->destroy(tempTex);
			}
			ImGui::EndDragDropTarget();
		}

		// Texture names
		for (int i = 0; i < Engine::textures.size(); i++)
		{
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex)
			{
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					const std::string newName(nameBuff);
					if (newName == Engine::textures[i]->name)
					{
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::textures, newName))
					{
						Engine::textures[i]->name = newName;
						nameIndex = -1;
					}
				}
			}
			// Draw
			else
			{
				ImGui::Selectable(Engine::textures[i]->name.c_str());
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem())
			{
				// Popup window width
				const float winWidth = ImGui::GetWindowContentRegionWidth();

				// Texture display
				ImGui::Image(Engine::textures[i]->view,
					ImVec2(winWidth, winWidth), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				// Rename
				if (ImGui::Button("Rename", ImVec2(winWidth, 0.0f)))
				{
					nameIndex = i;
					memcpy(nameBuff,
						Engine::textures[i]->name.c_str(),
						Engine::textures[i]->name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(winWidth, 0.0f)))
				{
					for (int j = 0; j < Engine::entities.size(); j++)
					{
						if (Engine::entities[j]->texture == Engine::textures[i])
							Engine::entities[j]->texture = Engine::Default::texture;
					}
					Engine::textures.delInst(Engine::textures[i]);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		// New texture popup
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

		// Color picker
		static kl::float3 textureColor;
		if (displayColorPicker)
		{
			if (ImGui::Begin("Texture color"))
			{
				ImGui::ColorPicker3("##Color picker", (float*)&textureColor);
				if (ImGui::Button("Done", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					ID3D11Texture2D* tempTex = Engine::Render::gpu->newTexture(
						kl::image(kl::int2(1), kl::convert::toColor(textureColor)));
					Engine::textures.newInst(new Engine::Texture("undefined",
						Engine::Render::gpu->newShaderView(tempTex)));
					Engine::Render::gpu->destroy(tempTex);
					displayColorPicker = false;
				}
				ImGui::End();
			}
		}
		ImGui::End();
	}
}

void Skyboxes()
{
	if (ImGui::Begin("Skyboxes", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		// Transfer
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		ImGuiID winId = ImGui::GetID("Skyboxes");
		if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId))
		{
			// Highlight
			if (ImGui::GetDragDropPayload()->IsDataType("TextureTransfer"))
				ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));

			// Payload accept
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextureTransfer"))
			{
				std::filesystem::path filePath((char*)payload->Data);
				Engine::skybox* loadedSB = Engine::skybox::newSkybox(Engine::Render::gpu,
					filePath.filename().stem().string(), kl::image(filePath.string()));
				if (loadedSB)
					Engine::skyboxes.newInst(loadedSB);
			}
			ImGui::EndDragDropTarget();
		}

		// Draw
		bool noSelection = true;
		for (int i = 0; i < Engine::skyboxes.size(); i++)
		{
			// Name input
			static int nameIndex = -1;
			static char nameBuff[64] = {};
			if (i == nameIndex)
			{
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					const std::string newName(nameBuff);
					if (newName == Engine::skyboxes[i]->name)
					{
						nameIndex = -1;
					}
					else if (!Engine::find(Engine::skyboxes, newName))
					{
						Engine::skyboxes[i]->name = newName;
						nameIndex = -1;
					}
				}
			}
			// Draw
			else
			{
				bool state = (Engine::Background::skybox == Engine::skyboxes[i]);
				ImGui::Selectable(Engine::skyboxes[i]->name.c_str(), &state);
				if (state)
				{
					Engine::Background::skybox = Engine::skyboxes[i];
					noSelection = false;
				}
			}

			// RMB menu
			if (ImGui::BeginPopupContextItem())
			{
				// Rename
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					nameIndex = i;
					memcpy(nameBuff,
						Engine::skyboxes[i]->name.c_str(),
						Engine::skyboxes[i]->name.size() + 1);
					ImGui::CloseCurrentPopup();
				}

				// Delete
				if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
				{
					if (Engine::Background::skybox == Engine::skyboxes[i])
						Engine::Background::skybox = nullptr;
					Engine::skyboxes.delInst(Engine::skyboxes[i]);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		if (noSelection)
			Engine::Background::skybox = nullptr;
		ImGui::End();
	}
}

void Engine::GUI::Scene()
{
	Entites();
	Meshes();
	Skyboxes();
	Textures();
}
