#include "GUI/GUI.h"
#include "Types/Entity.h"
#include "Stage/Stage.h"
#include "Editor/Picking.h"
#include "Render/Render.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"


static std::list<kl::reference<Engine::Entity>> savedEntities;
static std::vector<std::string> savedNames;

void Engine::GUI::ViewportRender() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
		ImGui::Image(Engine::Render::shaderView, ImVec2(float(Engine::Render::targetSize.x), float(Engine::Render::targetSize.y)));

		Engine::GUI::viewportFocus = ImGui::IsWindowHovered();
		bool hovers[3] = {};

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x * 0.5f - 22.0f, 10.0f));
		if (!Engine::gameRunning) {
			if (ImGui::Button("PLAY")) {
				for (auto& ent : Engine::entities) {
					savedEntities.push_back(kl::make<Engine::Entity>(*ent));
					savedNames.push_back(ent->getName());
				}
				Engine::Physics::CreateScene();
				Engine::JavaHandler::ReloadScripts();
				Engine::Scripting::CallStarts();
				Engine::Time::timer.reset();
				Engine::gameRunning = true;
			}
		}
		else {
			if (ImGui::Button("STOP")) {
				const std::string lastSelectedName = Engine::Selected::entity ? Engine::Selected::entity->getName() : "";
				Engine::Selected::entity = nullptr;
				Engine::Physics::DestroyScene();
				Engine::entities = savedEntities;
				savedEntities.clear();
				for (size_t i = 0; auto & ent : Engine::entities) {
					ent->updateName(savedNames[i++]);
				}
				savedNames.clear();
				for (auto& ent : Engine::entities) {
					if (ent->getName() == lastSelectedName) {
						Engine::Selected::entity = ent;
						break;
					}
				}
				Engine::gameRunning = false;
			}
		}
		hovers[0] = ImGui::IsItemHovered();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 80.0f, 10.0f));
		if (ImGui::ImageButton((Engine::Selected::raster == Engine::Rasters::solid) ? Engine::GUI::solidRaIcon : Engine::GUI::solidRaGIcon, ImVec2(20.0f, 20.0f))) {
			Engine::Selected::raster = Engine::Rasters::solid;
		}
		hovers[1] = ImGui::IsItemHovered();

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 42.0f, 10.0f));
		if (ImGui::ImageButton((Engine::Selected::raster == Engine::Rasters::wire) ? Engine::GUI::wireRaIcon : Engine::GUI::wireRaGIcon, ImVec2(20.0f, 20.0f))) {
			Engine::Selected::raster = Engine::Rasters::wire;
		}
		hovers[2] = ImGui::IsItemHovered();
		ImGui::PopStyleColor();

		Engine::GUI::viewportFocus = Engine::GUI::viewportFocus && !hovers[0] && !hovers[1] && !hovers[2];

		ImVec2 viewPos = ImGui::GetWindowPos();
		ImVec2 viewSize = ImGui::GetWindowSize();
		Engine::GUI::viewportPosition = kl::uint2(uint(viewPos.x), uint(viewPos.y));
		Engine::GUI::viewportSize = kl::uint2(uint(viewSize.x), uint(viewSize.y));
		ImGui::End();
	}
	ImGui::PopStyleVar();
}
