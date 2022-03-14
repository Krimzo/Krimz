#include "Engine/Engine.h"


int main() {
	// Hiding the console
	kl::console::hide();

	// Callbacks
	win.start = Start;
	win.update = Update;
	win.end = End;

	// Input setup
	InputSetup();

	// ImGui
	kl::igui::init();

	// Loading my theme
	kl::igui::loadKrimzTheme();

	// Time
	timer.reset();
	timer.interval();

	// Window
	win.startNew(kl::int2(1600, 900), "Editor", true, true, true);

	// Cleanup
	kl::igui::uninit();
}
