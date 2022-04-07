#include "Input/Input.h"


void Engine::Input::Setup()
{
	// LMB
	Engine::Input::LMB();

	// Gizmo input
	Engine::Input::Gizmo();

	// Free cam
	Engine::Input::Camera();
}
