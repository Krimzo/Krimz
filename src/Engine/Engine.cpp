#include "Engine/Engine.h"


int main() {
	// Hiding the console
	kl::console::hide();

	// Callbacks
	win.start = Start;
	win.update = Update;
	win.end = End;

	// Time
	timer.reset();
	timer.interval();

	// ImGui
	kl::igui::init();

	// Loading my theme
	kl::igui::loadKrimzTheme();

	// Window
	win.startNew(kl::ivec2(1600, 900), "Editor", true, true, true);

	// Cleanup
	kl::igui::uninit();
}
