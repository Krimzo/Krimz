#include "Engine/Engine.h"


int main() {
	// Callbacks
	win.start = Start;
	win.update = Update;
	win.end = End;

	// Time
	timer.reset();
	timer.interval();

	// ImGui
	kl::gui::init();

	// Window
	win.startNew(kl::ivec2(1600, 900), "Krimz Editor", false, false, true);

	// Cleanup
	kl::gui::uninit();
}
