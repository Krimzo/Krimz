#include "GUI/GUI.h"
#include "Data/Entities.h"
#include "Stage/Stage.h"
#include "Editor/Picking.h"
#include "Render/Render.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"


static std::list<std::shared_ptr<Engine::Entity>> savedEntities;
static std::vector<String> savedNames;

void Engine::GUI::Viewport() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		// Displaying render texture
		ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
		ImGui::Image(Engine::Render::shaderV, ImVec2(float(Engine::Render::targetSize.x), float(Engine::Render::targetSize.y)));

		// Focuse save
		Engine::GUI::viewportFocus = ImGui::IsWindowHovered();

		// Hovers
		bool hovers[3] = {};

		// Game start button
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x * 0.5f - 22.0f, 10.0f));
		if (!Engine::gameRunning) {
			if (ImGui::Button("PLAY")) {
				// Saving entites
				for (auto& ent : Engine::entities) {
					savedEntities.push_back(std::make_shared<Engine::Entity>(*ent));
					savedNames.push_back(ent->getName());
				}

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
				const String lastSelectedName = Engine::Picking::selected ? Engine::Picking::selected->getName() : "";

				// Physics scene cleanup
				Engine::Physics::DestroyScene();

				// Loading saved entities
				Engine::entities = savedEntities;
				savedEntities.clear();
				for (size_t i = 0; auto & ent : Engine::entities) {
					ent->updateName(savedNames[i++]);
				}
				savedNames.clear();

				// Selected fix
				for (auto& ent : Engine::entities) {
					if (ent->getName() == lastSelectedName) {
						Engine::Picking::selected = ent.get();
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
	ImGui::PopStyleVar();
}
