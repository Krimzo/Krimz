#include "Engine/GUI.h"


// GUI draw function
void GUI() {
	// Getting the win size
	winSize = win.getSize();

	// Main menu
	MainMenu();

	// Scene
	Scene();

	// Explorer
	Explorer();

	// Properties
	Properties();
}
