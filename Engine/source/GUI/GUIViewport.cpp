#include "GUI/GUI.h"
#include "Data/Entities.h"
#include "Stage/Stage.h"
#include "Input/Picking.h"
#include "Utility/Utility.h"
#include "Scripting/Scripting.h"
#include "Render/Render.h"
#include "Physics/Physics.h"


std::list<Engine::Entity> savedEntities;

void Engine::GUI::Viewport() {
	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground)) {
		// Focuse save
		Engine::GUI::viewportFocus = ImGui::IsWindowHovered();

		// Hovers
		bool hovers[3] = {};

		// Game start button
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x * 0.5f - 22.0f, 10.0f));
		if (!Engine::gameRunning) {
			if (ImGui::Button("PLAY")) {
				// Saving entites
				savedEntities = entities;

				// Physics scene creation
				Engine::Physics::CreateScene();

				// Reloading scripts
				Engine::JavaHandler::ReloadScripts();

				// Calling start scripts
				Engine::Scripting::CallStarts();

				// Other
				Engine::Time::timer.reset();
				Engine::gameRunning = true;
			}
		}
		else {
			if (ImGui::Button("STOP")) {
				// Saving selected's name
				const std::string lastSelectedName = Engine::Picking::selected ? Engine::Picking::selected->name : "";

				// Physics scene cleanup
				Engine::Physics::DestroyScene();

				// Loading saved entities
				Engine::entities = savedEntities;

				// Selected fix
				for (auto& ent : Engine::entities) {
					if (ent.name == lastSelectedName) {
						Engine::Picking::selected = &ent;
						break;
					}
				}

				// Other
				Engine::gameRunning = false;
			}
		}
		hovers[0] = ImGui::IsItemHovered();

		// Solid
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 80.0f, 10.0f));
		if (ImGui::ImageButton((Engine::Render::entityRaster == Engine::Rasters::solid) ? Engine::GUI::solidRaIcon : Engine::GUI::solidRaGIcon, ImVec2(20.0f, 20.0f))) {
			Engine::Render::entityRaster = Engine::Rasters::solid;
		}
		hovers[1] = ImGui::IsItemHovered();

		// Wireframe
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 42.0f, 10.0f));
		if (ImGui::ImageButton((Engine::Render::entityRaster == Engine::Rasters::wire) ? Engine::GUI::wireRaIcon : Engine::GUI::wireRaGIcon, ImVec2(20.0f, 20.0f))) {
			Engine::Render::entityRaster = Engine::Rasters::wire;
		}
		hovers[2] = ImGui::IsItemHovered();
		ImGui::PopStyleColor();

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
