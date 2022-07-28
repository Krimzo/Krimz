#include "GUI/GUI.h"
#include "Render/Render.h"


void Krimz::GUI::MainMenuRender()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				Krimz::window.stop();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Render"))
		{
			ImGui::Selectable("Colliders", &Krimz::Colliders::shouldRender);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
