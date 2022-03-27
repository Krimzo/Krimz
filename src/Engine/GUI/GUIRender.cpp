#include "Engine/Render/Render.h"
#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Window/Window.h"


void Engine::Render::GUI() {
	// Enabling main window docking
	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

	// Main menu
	Engine::GUI::MainMenu();

	// Scene
	Engine::GUI::Scene();

	// Viewport
	Engine::GUI::Viewport();

	// Explorer
	Engine::GUI::Explorer();

	// Log
	Engine::GUI::Log();

	// Properties
	Engine::GUI::Properties();
}
