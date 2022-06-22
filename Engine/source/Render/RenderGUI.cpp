#include "Render/Render.h"
#include "GUI/GUI.h"


void Engine::Render::GUI() {
	Engine::gpu->bindInternal();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

	Engine::GUI::MainMenuRender();
	Engine::GUI::SceneRender();
	Engine::GUI::ViewportRender();
	Engine::GUI::PerformanceRender();
	Engine::GUI::LogRender();
	Engine::GUI::ExplorerRender();
	Engine::GUI::PropertiesRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
