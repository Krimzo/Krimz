#include "GUI/GUI.h"
#include "Window/Window.h"
#include "Logging/Logging.h"
#include "Scripting/Scripting.h"
#include <algorithm>


static std::filesystem::path currentPath = std::filesystem::current_path();

void Engine::GUI::ExplorerRender() {
	if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		ImGuiStyle& style = ImGui::GetStyle();
		const float oldButt = style.Colors[ImGuiCol_Button].w;
		const float oldButtHov = style.Colors[ImGuiCol_ButtonHovered].w;
		const float oldButtAct = style.Colors[ImGuiCol_ButtonActive].w;
		style.Colors[ImGuiCol_Button].w = 0.0f;
		style.Colors[ImGuiCol_ButtonHovered].w = 0.25f;
		style.Colors[ImGuiCol_ButtonActive].w = 0.5f;

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

		const int columnCount = 12;
		ImGui::Columns(columnCount, nullptr, false);
		const float buttonSize = 0.75f * ImGui::GetWindowSize().x / columnCount;

		if (currentPath.has_parent_path()) {
			ImGui::PushID("__FolderBack");
			if (ImGui::ImageButton(Engine::GUI::folderIcon, ImVec2(buttonSize, buttonSize))) {
				currentPath = currentPath.parent_path();
			}
			ImGui::PopID();
			ImGui::TextWrapped("..");
			ImGui::NextColumn();
		}

		static std::filesystem::path toRename;
		static char nameBuff[64] = {};

		for (auto& folder : folders) {
			ImGui::PushID(folder.filename().string().c_str());
			ImTextureID folderIco = std::filesystem::is_empty(folder) ? Engine::GUI::folderEIcon : Engine::GUI::folderIcon;
			if (ImGui::ImageButton(folderIco, ImVec2(buttonSize, buttonSize))) {
				currentPath = folder;
			}
			ImGui::PopID();

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Button("Rename")) {
					toRename = folder;
					memcpy(nameBuff, folder.filename().string().c_str(), folder.filename().string().size() + 1);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					std::filesystem::remove_all(folder);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (folder == toRename) {
				ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					const std::string newName(nameBuff);
					memset(nameBuff, 0, sizeof(nameBuff));

					std::filesystem::rename(toRename,
						std::filesystem::path(toRename).replace_filename(newName));
					toRename.clear();
				}
			}
			else {
				ImGui::TextWrapped(folder.filename().string().c_str());
			}

			ImGui::NextColumn();
		}

		for (auto& file : files) {
			ImGui::PushID(file.filename().string().c_str());
			ImTextureID fileIco = Engine::GUI::fileIcon;
			const std::string fileExtension = file.filename().extension().string();
			if (fileExtension == ".obj") {
				fileIco = Engine::GUI::objectIcon;
			}
			else if (fileExtension == ".jpg" || fileExtension == ".png" || fileExtension == ".bmp") {
				fileIco = Engine::GUI::imageIcon;
			}
			else if (fileExtension == ".cpp" || fileExtension == ".h" || fileExtension == ".hlsl") {
				fileIco = Engine::GUI::codeIcon;
			}
			else if (fileExtension == ".java") {
				fileIco = Engine::GUI::scriptIcon;
			}
			if (ImGui::ImageButton(fileIco, ImVec2(buttonSize, buttonSize))) {
				ShellExecuteA(0, 0, file.string().c_str(), 0, 0, SW_SHOW);
			}
			ImGui::PopID();

			if (fileIco == Engine::GUI::objectIcon) {
				if (ImGui::BeginDragDropSource()) {
					const std::string filePath = file.string();
					ImGui::SetDragDropPayload("MeshTransfer", filePath.c_str(), filePath.size() + 1);
					ImGui::Image(fileIco, ImVec2(50.0f, 50.0f));
					ImGui::EndDragDropSource();
				}
			}
			else if (fileIco == Engine::GUI::imageIcon) {
				if (ImGui::BeginDragDropSource()) {
					const std::string filePath = file.string();
					ImGui::SetDragDropPayload("TextureTransfer", filePath.c_str(), filePath.size() + 1);
					ImGui::Image(fileIco, ImVec2(50.0f, 50.0f));
					ImGui::EndDragDropSource();
				}
			}
			else if (fileIco == Engine::GUI::scriptIcon) {
				if (ImGui::BeginDragDropSource()) {
					const std::string filePath = file.string();
					ImGui::SetDragDropPayload("ScriptTransfer", filePath.c_str(), filePath.size() + 1);
					ImGui::Image(fileIco, ImVec2(50.0f, 50.0f));
					ImGui::EndDragDropSource();
				}
			}

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Button("Rename", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					toRename = file;
					memcpy(nameBuff, file.filename().string().c_str(), file.filename().string().size() + 1);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
					std::filesystem::remove(file);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (file == toRename) {
				ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					const std::string newName(nameBuff);
					memset(nameBuff, 0, sizeof(nameBuff));

					std::filesystem::rename(toRename,
						std::filesystem::path(toRename).replace_filename(newName));
					toRename.clear();
				}
			}
			else {
				ImGui::TextWrapped(file.filename().string().c_str());
			}
			ImGui::NextColumn();
		}

		static bool namingNewScript = false;
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::Button("New Script", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
				namingNewScript = true;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("New Folder", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
				std::filesystem::create_directory(currentPath.string() + "/New Folder");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (namingNewScript) {
			if (ImGui::Begin("New Script Name", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar)) {
				ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
				if (ImGui::InputText("##NewName", nameBuff, sizeof(nameBuff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					const std::string newName(nameBuff);
					memset(nameBuff, 0, sizeof(nameBuff));
					std::ofstream file(currentPath.string() + "/" + newName + ".java");
					if (file.is_open()) {
						file << "import engine.*;\nimport engine.math.*;\nimport engine.script.*;\nimport engine.input.*;\nimport engine.entity.*;\n\n\npublic class " <<
							newName << " extends Entity implements Script {\n\n\t// Called on first frame\n\tpublic void start() {\n\n\t}\n\n\t// Called every frame\n\tpublic void update() {\n\n\t}\n}\n";
						file.close();
					}
					namingNewScript = false;
				}
				ImGui::End();
			}
		}
		style.Colors[ImGuiCol_Button].w = oldButt;
		style.Colors[ImGuiCol_ButtonHovered].w = oldButtHov;
		style.Colors[ImGuiCol_ButtonActive].w = oldButtAct;
		ImGui::End();
	}
}
