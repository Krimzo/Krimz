#include "Engine/Engine.h"


int main() {
	// Hiding the console
	kl::console::hide();

	// Callbacks
	Engine::Window::win.start = Engine::Stage::Start;
	Engine::Window::win.update = Engine::Stage::Update;
	Engine::Window::win.end = Engine::Stage::End;

	// Input setup
	Engine::Input::Setup();

	// ImGui
	kl::igui::init();

	// Loading my theme
	kl::igui::loadKrimzTheme();

	// Time
	Engine::Time::timer.reset();
	Engine::Time::timer.interval();

	// Window
	Engine::Window::win.startNew(kl::int2(1600, 900), "Editor", true, true, true);

	// Cleanup
	kl::igui::uninit();
}
