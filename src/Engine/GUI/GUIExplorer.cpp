#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Window/Window.h"
#include "Engine/Logging/Logging.h"
#include <algorithm>


std::filesystem::path currentPath = std::filesystem::current_path();

void Engine::GUI::Explorer() {
	if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transparency fix
		ImGuiStyle& style = ImGui::GetStyle();
		const float oldButt = style.Colors[ImGuiCol_Button].w;
		const float oldButtHov = style.Colors[ImGuiCol_ButtonHovered].w;
		const float oldButtAct = style.Colors[ImGuiCol_ButtonActive].w;
		style.Colors[ImGuiCol_Button].w = 0.0f;
		style.Colors[ImGuiCol_ButtonHovered].w = 0.25f;
		style.Colors[ImGuiCol_ButtonActive].w = 0.5f;

		// Getting all content
		std::vector<std::filesystem::path> folders;
		std::vector<std::filesystem::path> files;
		for (auto& cont : std::filesystem::directory_iterator(currentPath)) {
			if (cont.is_directory()) {
				folders.push_back(cont.path());
			}
			else {
				files.push_back(cont.path());
			}
		}
		std::sort(folders.begin(), folders.end());
		std::sort(files.begin(), files.end());

		// Column setup
		const int columnCount = 12;
		ImGui::Columns(columnCount, nullptr, false);
		const float buttonSize = 0.75f * ImGui::GetWindowSize().x / columnCount;

		// Drawing parent folder
		if (currentPath.has_parent_path()) {
			ImGui::PushID("__FolderBack");
			if (ImGui::ImageButton(Engine::GUI::folderIcon, ImVec2(buttonSize, buttonSize))) {
				currentPath = currentPath.parent_path();
			}
			ImGui::PopID();
			ImGui::TextWrapped("..");
			ImGui::NextColumn();
		}

		// Drawing folders
		for (auto& folder : folders) {
			// Button draw
			ImGui::PushID(folder.filename().string().c_str());
			ImTextureID folderIco = std::filesystem::is_empty(folder) ? Engine::GUI::folderEIcon : Engine::GUI::folderIcon;
			if (ImGui::ImageButton(folderIco, ImVec2(buttonSize, buttonSize))) {
				currentPath = folder;
			}
			ImGui::PopID();
			ImGui::TextWrapped(folder.filename().string().c_str());
			ImGui::NextColumn();
		}

		// Drawing files
		for (auto& file : files) {
			// Button draw
			ImGui::PushID(file.filename().string().c_str());
			ImTextureID fileIco = Engine::GUI::fileIcon;
			const std::string fileExtension = file.filename().extension().string();
			if (fileExtension == ".obj") {
				fileIco = Engine::GUI::objectIcon;
			}
			else if (fileExtension == ".jpg" || fileExtension == ".png" || fileExtension == ".bmp") {
				fileIco = Engine::GUI::imageIcon;
			}
			else if (fileExtension == ".cpp" || fileExtension == ".java" || fileExtension == ".hlsl") {
				fileIco = Engine::GUI::codeIcon;
			}
			else if (fileExtension == ".class") {
				fileIco = Engine::GUI::scriptIcon;
			}
			if (ImGui::ImageButton(fileIco, ImVec2(buttonSize, buttonSize))) {
				ShellExecuteA(0, 0, file.string().c_str(), 0, 0, SW_SHOW);
			}

			// Transfer
			if (fileIco == Engine::GUI::objectIcon) {
				if (ImGui::BeginDragDropSource()) {
					std::string filePath = file.string();
					ImGui::SetDragDropPayload("MeshTransfer", filePath.c_str(), filePath.size() + 1);
					ImGui::EndDragDropSource();
				}
			}
			else if (fileIco == Engine::GUI::imageIcon) {
				if (ImGui::BeginDragDropSource()) {
					std::string filePath = file.string();
					ImGui::SetDragDropPayload("TextureTransfer", filePath.c_str(), filePath.size() + 1);
					ImGui::EndDragDropSource();
				}
			}
			else if (fileIco == Engine::GUI::scriptIcon) {
				if (ImGui::BeginDragDropSource()) {
					std::string filePath = file.string();
					ImGui::SetDragDropPayload("ScriptTransfer", filePath.c_str(), filePath.size() + 1);
					ImGui::EndDragDropSource();
				}
			}

			// Next col
			ImGui::PopID();
			ImGui::TextWrapped(file.filename().string().c_str());
			ImGui::NextColumn();
		}

		// Style reset
		style.Colors[ImGuiCol_Button].w = oldButt;
		style.Colors[ImGuiCol_ButtonHovered].w = oldButtHov;
		style.Colors[ImGuiCol_ButtonActive].w = oldButtAct;

		// End draw
		ImGui::End();
	}
}
