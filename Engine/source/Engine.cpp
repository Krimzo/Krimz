#include "KrimzLib.h"
#include "Stage/Stage.h"
#include "Input/Input.h"
#include "Render/Render.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"
#include "GUI/GUI.h"


int main() {
	// Callback setup
	Engine::win.start = Engine::Stage::Start;
	Engine::win.update = Engine::Stage::Update;
	Engine::win.end = Engine::Stage::End;

	// Input setup
	Engine::Input::Setup();

	// GUI setup
	Engine::GUI::Init();
	Engine::GUI::LoadKrimzTheme();

	// Script setup
	Engine::JavaHandler::Init();

	// Physics setup
	Engine::Physics::Init();

	// Time
	Engine::Time::timer.reset();
	Engine::Time::timer.interval();

	// Start
	Engine::win.startNew(kl::int2(1600, 900), "Editor", true, true);
}
