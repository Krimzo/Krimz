#include "Engine/GUI/GUI.h"


void Engine::Update::GUIBefore() {
	// Main menu
	Engine::GUI::MainMenu();

	// Scene
	Engine::GUI::Scene();

	// Explorer
	Engine::GUI::Explorer();

	// Properties
	Engine::GUI::Properties();

	// Viewport save
	Engine::GUI::viewportPos = kl::int2(Engine::GUI::explorPos.x, Engine::GUI::scenePos.y);
	Engine::GUI::viewportSize = kl::int2(Engine::GUI::explorSize.x, Engine::Window::size.y - Engine::GUI::explorSize.y - int(Engine::GUI::mMenuSize.y));
}
