#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Window/Window.h"
#include <algorithm>


std::filesystem::path currentPath = std::filesystem::current_path();

void Engine::GUI::Explorer() {
	if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Transparency fix
		ImGuiStyle& style = ImGui::GetStyle();
		const ImVec4 oldButt = style.Colors[ImGuiCol_Button];
		const ImVec4 oldButtHov = style.Colors[ImGuiCol_ButtonHovered];
		const ImVec4 oldButtAct = style.Colors[ImGuiCol_ButtonActive];
		style.Colors[ImGuiCol_Button] = ImVec4(0.75f, 0.40f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.75f, 0.40f, 0.0f, 0.25f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.45f, 0.0f, 0.40f);

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
			if (ImGui::ImageButton(Engine::GUI::folderTex, ImVec2(buttonSize, buttonSize))) {
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
			if (ImGui::ImageButton(std::filesystem::is_empty(folder) ?
				Engine::GUI::emptyFolderTex : Engine::GUI::folderTex,
				ImVec2(buttonSize, buttonSize))) {
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
			if (ImGui::ImageButton(Engine::GUI::fileTex, ImVec2(buttonSize, buttonSize))) {

			}
			ImGui::PopID();
			ImGui::TextWrapped(file.filename().string().c_str());
			ImGui::NextColumn();
		}

		// Style reset
		style.Colors[ImGuiCol_Button] = oldButt;
		style.Colors[ImGuiCol_ButtonHovered] = oldButtHov;
		style.Colors[ImGuiCol_ButtonActive] = oldButtAct;

		// End draw
		ImGui::End();
	}
}
