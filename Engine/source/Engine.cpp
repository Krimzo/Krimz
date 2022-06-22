#include "KrimzLib.h"
#include "Stage/Stage.h"
#include "Input/Input.h"
#include "Render/Render.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"
#include "GUI/GUI.h"


int main() {
	Engine::window.start = Engine::Stage::Start;
	Engine::window.update = Engine::Stage::Update;
	Engine::window.end = Engine::Stage::End;

	Engine::Input::Setup();

	Engine::GUI::Init();
	Engine::GUI::LoadKrimzTheme();

	Engine::JavaHandler::Init();

	Engine::Physics::Init();

	Engine::Time::timer.reset();
	Engine::Time::timer.interval();

	Engine::window.run({ 1600, 900 }, "Editor", true, true);

	Engine::Physics::Uninit();

	Engine::JavaHandler::Uninit();
}
