#include "Engine/GUI.h"


// GUI draw function
void GUI() {
	// Getting the win size
	winSize = win.getSize();

	// Full window
	gpu->viewport(kl::int2(0), winSize);

	// Main menu
	MainMenu();

	// Scene
	Scene();

	// Explorer
	Explorer();

	// Properties
	Properties();

	// Viewport save
	guiViewportPos = kl::int2(guiExplorPos.x, guiScenePos.y);
	guiViewportSize = kl::int2(guiExplorSize.x, winSize.y - guiExplorSize.y - int(mainMenuSize.y));
}
