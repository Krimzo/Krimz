#include "Engine/Engine.h"

#include "ImGui/imgui.h"


int main() {
	// Callbacks
	win.start = Start;
	win.update = Update;
	win.end = End;

	// Time
	timer.reset();
	timer.interval();

	// ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Window
	win.startNew(kl::ivec2(1600, 900), "Krimz Editor", false, false);

	// Cleanup
	ImGui::DestroyContext();
}
