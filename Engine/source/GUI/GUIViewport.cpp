#include "GUI/GUI.h"
#include "Types/Entity.h"
#include "Stage/Stage.h"
#include "Editor/Picking.h"
#include "Render/Render.h"
#include "Scripting/Scripting.h"
#include "Physics/Physics.h"


static std::list<kl::ref<Krimz::Entity>> savedEntities;
static std::vector<std::string> savedNames;

void Krimz::GUI::ViewportRender() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
		ImGui::Image(Krimz::Render::shaderView, ImVec2(float(Krimz::Render::targetSize.x), float(Krimz::Render::targetSize.y)));

		Krimz::GUI::viewportFocus = ImGui::IsWindowHovered();
		bool hovers[3] = {};

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x * 0.5f - 22.0f, 10.0f));
		if (!Krimz::gameRunning) {
			if (ImGui::Button("PLAY")) {
				for (auto& ent : Krimz::entities) {
					savedEntities.push_back(kl::make<Krimz::Entity>(*ent));
					savedNames.push_back(ent->getName());
				}
				Krimz::Physics::CreateScene();
				Krimz::JavaHandler::ReloadScripts();
				Krimz::Scripting::CallStarts();
				//Krimz::Time::timer.reset();
				Krimz::gameRunning = true;
			}
		}
		else {
			if (ImGui::Button("STOP")) {
				const std::string lastSelectedName = Krimz::Selected::entity ? Krimz::Selected::entity->getName() : "";
				Krimz::Selected::entity = nullptr;
				Krimz::Physics::DestroyScene();
				Krimz::entities = savedEntities;
				savedEntities.clear();
				for (size_t i = 0; auto & ent : Krimz::entities) {
					ent->updateName(savedNames[i++]);
				}
				savedNames.clear();
				for (auto& ent : Krimz::entities) {
					if (ent->getName() == lastSelectedName) {
						Krimz::Selected::entity = ent;
						break;
					}
				}
				Krimz::gameRunning = false;
			}
		}
		hovers[0] = ImGui::IsItemHovered();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 80.0f, 10.0f));
		if (ImGui::ImageButton((Krimz::Selected::raster == Krimz::Rasters::solid) ? Krimz::GUI::solidRaIcon : Krimz::GUI::solidRaGIcon, ImVec2(20.0f, 20.0f))) {
			Krimz::Selected::raster = Krimz::Rasters::solid;
		}
		hovers[1] = ImGui::IsItemHovered();

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 42.0f, 10.0f));
		if (ImGui::ImageButton((Krimz::Selected::raster == Krimz::Rasters::wire) ? Krimz::GUI::wireRaIcon : Krimz::GUI::wireRaGIcon, ImVec2(20.0f, 20.0f))) {
			Krimz::Selected::raster = Krimz::Rasters::wire;
		}
		hovers[2] = ImGui::IsItemHovered();
		ImGui::PopStyleColor();

		Krimz::GUI::viewportFocus = Krimz::GUI::viewportFocus && !hovers[0] && !hovers[1] && !hovers[2];

		ImVec2 viewPos = ImGui::GetWindowPos();
		ImVec2 viewSize = ImGui::GetWindowSize();
		Krimz::GUI::viewportPosition = kl::uint2(uint(viewPos.x), uint(viewPos.y));
		Krimz::GUI::viewportSize = kl::uint2(uint(viewSize.x), uint(viewSize.y));
		ImGui::End();
	}
	ImGui::PopStyleVar();
}
