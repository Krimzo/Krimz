#include "Render/Render.h"
#include "GUI/GUI.h"


void Engine::Render::GUI() {
    // Draw start
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Enabling main window docking
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

    // Main menu
    Engine::GUI::MainMenu();

    // Scene
    Engine::GUI::Scene();

    // Viewport
    Engine::GUI::Viewport();

    // Performance
    Engine::GUI::Performance();

    // Log
    Engine::GUI::Log();

    // Explorer
    Engine::GUI::Explorer();

    // Properties
    Engine::GUI::Properties();

    // Draw end
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
