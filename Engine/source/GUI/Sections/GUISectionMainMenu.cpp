#include "GUI/Sections/GUISections.h"


static bool Start() {
	return ImGui::BeginMainMenuBar();
}

static void Render() {
	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Exit")) {
			//Krimz::window.stop();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Render")) {
		//ImGui::Selectable("Colliders", &Krimz::Colliders::shouldRender);
		ImGui::EndMenu();
	}
}

static void End() {
	ImGui::EndMainMenuBar();
}

kl::ref<Krimz::GUISection> Krimz::GUISections::MainMenu() {
	kl::ref<GUISection> section = kl::make<GUISection>();
	section->start = Start;
	section->render = Render;
	section->end = End;
	return section;
}
