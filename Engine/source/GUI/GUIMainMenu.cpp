#include "GUI/GUI.h"
#include "Utility/Window.h"
#include "View/Background.h"


void Engine::GUI::MainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		// File
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				Engine::win.stop();

			ImGui::EndMenu();
		}

		// End draw
		ImGui::EndMainMenuBar();
	}
}
