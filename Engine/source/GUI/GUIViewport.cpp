#include "GUI/GUI.h"
#include "Data/Entities.h"
#include "Stage/Stage.h"
#include "Input/Picking.h"
#include "Utility/Time.h"
#include "Scripting/Scripting.h"
#include "Render/Render.h"


std::vector<Engine::Entity> savedEntities;

void Engine::GUI::Viewport()
{
	// Window draw
	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground))
	{
		// Focuse save
		Engine::GUI::viewportFocus = ImGui::IsWindowHovered();

		// Hovers
		bool hovers[3] = {};

		// Play button
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x * 0.5f - 22.0f, 10.0f));
		if (!Engine::gameRunning)
		{
			// Button draw
			if (ImGui::Button("PLAY"))
			{
				// Saving entites
				savedEntities.resize(Engine::entities.size());
				for (int i = 0; i < Engine::entities.size(); i++)
					savedEntities[i] = *Engine::entities[i];

				// Reloading scripts
				Engine::JavaHandler::ReloadScripts();

				// Calling start scripts
				Engine::Scripting::CallStarts();

				// Other
				Engine::Time::timer.reset();
				Engine::gameRunning = true;
			}
		}
		else
		{
			// Button draw
			if (ImGui::Button("STOP"))
			{
				// Saving selected's name
				const std::string lastSelectedName = Engine::Picking::selected ? Engine::Picking::selected->name : "";

				// Loading saved entities
				Engine::entities.clear();
				for (Engine::Entity& ent : savedEntities)
					Engine::entities.newInst(new Engine::Entity(ent));
				savedEntities.clear();

				// Selected fix
				for (int i = 0; i < Engine::entities.size(); i++)
				{
					if (Engine::entities[i]->name == lastSelectedName)
					{
						Engine::Picking::selected = Engine::entities[i];
						break;
					}
				}

				// Other
				Engine::gameRunning = false;
			}
		}
		hovers[0] = ImGui::IsItemHovered();

		// Solid
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 80.0f, 10.0f));
		if (ImGui::ImageButton((Engine::Render::entityRaster == Engine::Rasters::solid) ? Engine::GUI::solidRaIcon : Engine::GUI::solidRaGIcon, ImVec2(20.0f, 20.0f)))
			Engine::Render::entityRaster = Engine::Rasters::solid;
		hovers[1] = ImGui::IsItemHovered();

		// Wireframe
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 42.0f, 10.0f));
		if (ImGui::ImageButton((Engine::Render::entityRaster == Engine::Rasters::wire) ? Engine::GUI::wireRaIcon : Engine::GUI::wireRaGIcon, ImVec2(20.0f, 20.0f)))
			Engine::Render::entityRaster = Engine::Rasters::wire;
		hovers[2] = ImGui::IsItemHovered();

		// Button focus fix
		Engine::GUI::viewportFocus = Engine::GUI::viewportFocus && !hovers[0] && !hovers[1] && !hovers[2];

		// Saving viewport props
		ImVec2 viewPos = ImGui::GetWindowPos();
		ImVec2 viewSize = ImGui::GetWindowSize();
		Engine::GUI::viewportPos = kl::int2(int(viewPos.x), int(viewPos.y));
		Engine::GUI::viewportSize = kl::int2(int(viewSize.x), int(viewSize.y));

		// End draw
		ImGui::End();
	}
}
