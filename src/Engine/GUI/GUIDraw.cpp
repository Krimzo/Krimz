#include "Engine/GUI/GUIDraw.h"


// New ImGui frame
void Engine::GUI::StartDraw() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

// ImGui data render
void Engine::GUI::EndDraw() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
