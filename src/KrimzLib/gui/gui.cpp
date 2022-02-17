#include "KrimzLib/gui/gui.h"


// Inits the ImGui context
void kl::gui::init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

// Destroys the ImGui context
void kl::gui::uninit() {
	ImGui::DestroyContext();
}

// Draws the ImGui data
void kl::gui::draw(const std::function<void()>& func) {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	func();
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
