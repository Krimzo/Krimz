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
#include "ImGui/imgui_internal.h"


void Engine::GUI::Scene() {
	// Allignement
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

	// Entities
	if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Draw
		bool noSelection = true;
		for (int i = 0; i < Engine::Game::entities.size(); i++) {
			bool state = (Engine::Picking::selected == Engine::Game::entities[i]);
			ImGui::Selectable(Engine::Game::entities[i]->name.c_str(), &state);
			if (state) {
				Engine::Picking::selected = Engine::Game::entities[i];
				noSelection = false;
			}
		}
		if (noSelection) {
			Engine::Picking::selected = nullptr;
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

		// Draw
		for (int i = 0; i < Engine::Data::meshes.size(); i++) {
			ImGui::Text(Engine::Data::meshes[i]->name.c_str());
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

		// Draw
		for (int i = 0; i < Engine::Data::textures.size(); i++) {
			ImGui::Text(Engine::Data::textures[i]->name.c_str());
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
			}
			ImGui::EndDragDropTarget();
		}

		// Draw
		for (int i = 0; i < Engine::Scripting::handler->scripts.size(); i++) {
			ImGui::Text(Engine::Scripting::handler->scripts[i]->getName().c_str());
		}

		// End child window
		ImGui::End();
	}

	// Style pop
	ImGui::PopStyleVar();
}
