#include "Engine/GUI/GUI.h"


void Engine::GUI::ViewportOverlay() {
	// Saving old
	ImGuiStyle& style = ImGui::GetStyle();
	const ImVec4 oldColor = style.Colors[ImGuiCol_WindowBg];

	// Transparent setting
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	// Window draw
	ImGui::SetNextWindowPos(ImVec2(float(Engine::GUI::viewportPos.x), float(Engine::GUI::viewportPos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(Engine::GUI::viewportSize.x), float(Engine::GUI::viewportSize.y)));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("Viewport Overlay", nullptr, Engine::GUI::panelFlags | ImGuiWindowFlags_NoTitleBar)) {
		// Focuse save
		Engine::GUI::viewportFocus = ImGui::IsWindowHovered();
		
		// Play button
		if (!Engine::Game::running) {
			// Button draw
			if (ImGui::Button("PLAY")) {
				Engine::Game::running = true;
			}
		}
		else {
			// Button draw
			if (ImGui::Button("STOP")) {
				Engine::Game::running = false;
			}
		}

		// Button focus fix
		Engine::GUI::viewportFocus = Engine::GUI::viewportFocus && !ImGui::IsItemHovered();

		// End draw
		ImGui::End();
	}
	ImGui::PopStyleVar();

	// Old color reset
	style.Colors[ImGuiCol_WindowBg] = oldColor;
}
