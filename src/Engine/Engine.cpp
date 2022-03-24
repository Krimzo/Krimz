#include "KrimzLib/KrimzLib.hpp"

#include "Engine/Window/Window.h"
#include "Engine/Stage/Stage.h"
#include "Engine/Time/Time.h"
#include "Engine/Input/Input.h"
#include "Engine/GUI/GUISetup.h"
#include "Engine/Scripting/Scripting.h"


int main() {
	// Console setup
	kl::console::hide();

	// Callback setup
	Engine::Window::win.start = Engine::Stage::Start;
	Engine::Window::win.update = Engine::Stage::Update;
	Engine::Window::win.end = Engine::Stage::End;

	// Input setup
	Engine::Input::Setup();

	// GUI setup
	Engine::GUI::Init();
	Engine::GUI::LoadKrimzTheme();

	// Script setup
	Engine::Scripting::handler = new Engine::ScriptHandler();

	// Time
	Engine::Time::timer.reset();
	Engine::Time::timer.interval();

	// Window start
	Engine::Window::win.startNew(kl::int2(1600, 900), "Editor", true, true, true);
}
