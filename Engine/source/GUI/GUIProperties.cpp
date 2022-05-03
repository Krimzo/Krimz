#include "GUI/GUI.h"
#include "Editor/Picking.h"
#include "Scripting/Scripting.h"
#include "Stage/Stage.h"
#include "Physics/Physics.h"
#include "Render/Render.h"


void PropMeshPick(const std::shared_ptr<Engine::Mesh>& mesh) {
	bool selectedOne = (Engine::Selected::entity->mesh == mesh);
	if (ImGui::Selectable(mesh->getName().c_str(), selectedOne)) {
		if (selectedOne) {
			ImGui::SetItemDefaultFocus();
		}
		else {
			Engine::Selected::entity->mesh = mesh;
		}
	}
}
void PropGeometry() {
	if (ImGui::Begin("Geometry", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Selected::entity) {
			// Axis
			ImGui::DragFloat3("Scale", (float*)&Engine::Selected::entity->scale, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat3("Rotation", (float*)&Engine::Selected::entity->rotation, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat3("Position", (float*)&Engine::Selected::entity->position, 0.1f, 0.0f, 0.0f, "%.2f");

			// Mesh
			if (ImGui::BeginCombo("Mesh", Engine::Selected::entity->mesh->getName().c_str())) {
				for (auto& mes : Engine::meshes) {
					PropMeshPick(mes);
				}
				PropMeshPick(Engine::Default::cube);
				PropMeshPick(Engine::Default::sphere);
				PropMeshPick(Engine::Default::capsule);
				PropMeshPick(Engine::Default::pyramid);
				PropMeshPick(Engine::Default::monke);
				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}
}

void PropTexPick(const std::shared_ptr<Engine::Texture>& texture) {
	bool selectedOne = (Engine::Selected::entity->texture == texture);
	if (ImGui::Selectable(texture->getName().c_str(), selectedOne)) {
		if (selectedOne) {
			ImGui::SetItemDefaultFocus();
		}
		else {
			Engine::Selected::entity->texture = texture;
		}
	}
}
void PropView() {
	if (ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Selected::entity) {
			ImGui::Checkbox("Visible", &Engine::Selected::entity->visible);
			ImGui::Checkbox("Shadows", &Engine::Selected::entity->shadows);
			if (ImGui::BeginCombo("Texture", Engine::Selected::entity->texture->getName().c_str())) {
				for (auto& tex : Engine::textures) {
					PropTexPick(tex);
				}
				PropTexPick(Engine::Default::texture);
				ImGui::EndCombo();
			}
			ImGui::SliderFloat("Roughness", &Engine::Selected::entity->roughness, 0.0f, 1.0f, "%.2f");
		}
		ImGui::End();
	}
}

void PropPhysCollider() {
	if (ImGui::BeginCombo("Collider", Engine::Collider::getName(Engine::Selected::entity->collider.shape).c_str())) {
		if (ImGui::Selectable("None", Engine::Selected::entity->collider.shape == Engine::Collider::Shape::None)) {
			Engine::Selected::entity->collider.shape = Engine::Collider::Shape::None;
		}
		if (ImGui::Selectable("Box", Engine::Selected::entity->collider.shape == Engine::Collider::Shape::Box)) {
			Engine::Selected::entity->collider.shape = Engine::Collider::Shape::Box;
		}
		if (ImGui::Selectable("Sphere", Engine::Selected::entity->collider.shape == Engine::Collider::Shape::Sphere)) {
			Engine::Selected::entity->collider.shape = Engine::Collider::Shape::Sphere;
		}
		if (ImGui::Selectable("Capsule", Engine::Selected::entity->collider.shape == Engine::Collider::Shape::Capsule)) {
			Engine::Selected::entity->collider.shape = Engine::Collider::Shape::Capsule;
		}
		if (ImGui::Selectable("Mesh", Engine::Selected::entity->collider.shape == Engine::Collider::Shape::Mesh)) {
			Engine::Selected::entity->collider.shape = Engine::Collider::Shape::Mesh;
		}
		ImGui::EndCombo();
	}
	switch (Engine::Selected::entity->collider.shape) {
	case Engine::Collider::Shape::Sphere:
	{
		ImGui::DragFloat("Radius", (float*)&Engine::Selected::entity->collider.scale, 0.1f, 0.0f, 0.0f, "%.2f");
		Engine::Selected::entity->collider.scale = kl::float3(Engine::Selected::entity->collider.scale.x);
		ImGui::DragFloat3("Position", (float*)&Engine::Selected::entity->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;

	case Engine::Collider::Shape::Capsule:
	{
		kl::float2 radHei(Engine::Selected::entity->collider.scale.y, Engine::Selected::entity->collider.scale.x);
		ImGui::DragFloat2("Radius/Height", (float*)&radHei, 0.1f, 0.0f, 0.0f, "%.2f");
		Engine::Selected::entity->collider.scale.x = radHei.y;
		Engine::Selected::entity->collider.scale.y = radHei.x;
		Engine::Selected::entity->collider.scale.z = radHei.x;
		ImGui::DragFloat3("Rotation", (float*)&Engine::Selected::entity->collider.rotation, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Position", (float*)&Engine::Selected::entity->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;

	case Engine::Collider::Shape::Box:
	case Engine::Collider::Shape::Mesh:
	{
		ImGui::DragFloat3("Scale", (float*)&Engine::Selected::entity->collider.scale, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Rotation", (float*)&Engine::Selected::entity->collider.rotation, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Position", (float*)&Engine::Selected::entity->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;
	}
}
void PropPhysics() {
	if (ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Selected::entity) {
			ImGui::Checkbox("Dynamic", &Engine::Selected::entity->dynamic);
			if (Engine::Selected::entity->dynamic) {
				ImGui::Checkbox("Gravity", &Engine::Selected::entity->gravity);
			}
			ImGui::SliderFloat("Friction", &Engine::Selected::entity->friction, 0.0f, 1.0f, "%.2f");
			if (Engine::Selected::entity->dynamic) {
				ImGui::DragFloat("Mass", &Engine::Selected::entity->mass, 0.1f, 0.0f, 1000000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::DragFloat3("Velocity", (float*)&Engine::Selected::entity->velocity, 0.1f, 0.0f, 0.0f, "%.2f");
				ImGui::DragFloat3("Angular", (float*)&Engine::Selected::entity->angular, 0.1f, 0.0f, 0.0f, "%.2f");
			}
			PropPhysCollider();
		}
		ImGui::End();
	}
}

void PropScripts() {
	if (ImGui::Begin("Scripts", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Selected::entity) {
			// Transfer
			ImVec2 winPos = ImGui::GetWindowPos();
			ImVec2 winSize = ImGui::GetWindowSize();
			ImGuiID winId = ImGui::GetID("Scripts");
			if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId)) {
				// Highlight
				if (ImGui::GetDragDropPayload()->IsDataType("ScriptTransfer")) {
					ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
				}

				// Payload accept
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ScriptTransfer")) {
					std::filesystem::path filePath((char*)payload->Data);
					Engine::Selected::entity->scripts.push_back(std::make_shared<Engine::Script>(filePath.string()));
				}
				ImGui::EndDragDropTarget();
			}

			// Script names
			for (int i = 0; i < Engine::Selected::entity->scripts.size(); i++) {
				// Draw
				ImGui::PushID(i);
				ImGui::Selectable(std::filesystem::path(Engine::Selected::entity->scripts[i]->path).stem().string().c_str());
				ImGui::PopID();

				// RMB menu
				if (ImGui::BeginPopupContextItem()) {
					// Delete
					if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
						Engine::Selected::entity->scripts.erase(Engine::Selected::entity->scripts.begin() + i);
						ImGui::CloseCurrentPopup();
					}

					// End
					ImGui::EndPopup();
				}
			}
		}

		// Script reload
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::Button("Reload", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
				Engine::JavaHandler::ReloadScripts();
				ImGui::CloseCurrentPopup();
			}

			// End
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}

void Engine::GUI::Properties() {
	PropGeometry();
	PropView();
	PropPhysics();
	PropScripts();
}
