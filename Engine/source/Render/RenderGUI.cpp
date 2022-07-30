#include "Render/Render.h"
#include "GUI/GUI.h"


void Krimz::Render::GUI() {
	Krimz::gpu->bindInternal();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

	Krimz::GUI::MainMenuRender();
	Krimz::GUI::SceneRender();
	Krimz::GUI::ViewportRender();
	Krimz::GUI::PerformanceRender();
	Krimz::GUI::LogRender();
	Krimz::GUI::ExplorerRender();
	Krimz::GUI::PropertiesRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
