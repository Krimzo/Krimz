#include "GUI/GUI.h"
#include "Editor/Picking.h"
#include "Scripting/Scripting.h"
#include "Stage/Stage.h"
#include "Physics/Physics.h"
#include "Render/Render.h"


void PropMeshPick(Engine::Mesh* mesh) {
	bool selectedOne = (Engine::Picking::selected->mesh == mesh);
	if (ImGui::Selectable(mesh->getName().c_str(), selectedOne)) {
		if (selectedOne) {
			ImGui::SetItemDefaultFocus();
		}
		else {
			Engine::Picking::selected->mesh = mesh;
		}
	}
}
void PropGeometry() {
	if (ImGui::Begin("Geometry", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			// Axis
			ImGui::DragFloat3("Scale", (float*)&Engine::Picking::selected->scale, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat3("Rotation", (float*)&Engine::Picking::selected->rotation, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->position, 0.1f, 0.0f, 0.0f, "%.2f");

			// Mesh
			if (ImGui::BeginCombo("Mesh", Engine::Picking::selected->mesh->getName().c_str())) {
				for (auto& mes : Engine::meshes) {
					PropMeshPick(mes.get());
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

void PropTexPick(Engine::Texture* texture) {
	bool selectedOne = (Engine::Picking::selected->texture == texture);
	if (ImGui::Selectable(texture->getName().c_str(), selectedOne)) {
		if (selectedOne) {
			ImGui::SetItemDefaultFocus();
		}
		else {
			Engine::Picking::selected->texture = texture;
		}
	}
}
void PropView() {
	if (ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			ImGui::Checkbox("Visible", &Engine::Picking::selected->visible);
			ImGui::Checkbox("Shadows", &Engine::Picking::selected->shadows);
			if (ImGui::BeginCombo("Texture", Engine::Picking::selected->texture->getName().c_str())) {
				for (auto& tex : Engine::textures) {
					PropTexPick(tex.get());
				}
				PropTexPick(Engine::Default::texture);
				ImGui::EndCombo();
			}
			ImGui::SliderFloat("Roughness", &Engine::Picking::selected->roughness, 0.0f, 1.0f, "%.2f");
		}
		ImGui::End();
	}
}

void PropPhysCollider() {
	if (ImGui::BeginCombo("Collider", Engine::Collider::getName(Engine::Picking::selected->collider.shape).c_str())) {
		if (ImGui::Selectable("None", Engine::Picking::selected->collider.shape == Engine::Collider::Shape::None)) {
			Engine::Picking::selected->collider.shape = Engine::Collider::Shape::None;
		}
		if (ImGui::Selectable("Box", Engine::Picking::selected->collider.shape == Engine::Collider::Shape::Box)) {
			Engine::Picking::selected->collider.shape = Engine::Collider::Shape::Box;
		}
		if (ImGui::Selectable("Sphere", Engine::Picking::selected->collider.shape == Engine::Collider::Shape::Sphere)) {
			Engine::Picking::selected->collider.shape = Engine::Collider::Shape::Sphere;
		}
		if (ImGui::Selectable("Capsule", Engine::Picking::selected->collider.shape == Engine::Collider::Shape::Capsule)) {
			Engine::Picking::selected->collider.shape = Engine::Collider::Shape::Capsule;
		}
		if (ImGui::Selectable("Mesh", Engine::Picking::selected->collider.shape == Engine::Collider::Shape::Mesh)) {
			Engine::Picking::selected->collider.shape = Engine::Collider::Shape::Mesh;
		}
		ImGui::EndCombo();
	}
	switch (Engine::Picking::selected->collider.shape) {
	case Engine::Collider::Shape::Sphere:
	{
		ImGui::DragFloat("Radius", (float*)&Engine::Picking::selected->collider.scale, 0.1f, 0.0f, 0.0f, "%.2f");
		Engine::Picking::selected->collider.scale = kl::float3(Engine::Picking::selected->collider.scale.x);
		ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;

	case Engine::Collider::Shape::Capsule:
	{
		kl::float2 radHei(Engine::Picking::selected->collider.scale.y, Engine::Picking::selected->collider.scale.x);
		ImGui::DragFloat2("Radius/Height", (float*)&radHei, 0.1f, 0.0f, 0.0f, "%.2f");
		Engine::Picking::selected->collider.scale.x = radHei.y;
		Engine::Picking::selected->collider.scale.y = radHei.x;
		Engine::Picking::selected->collider.scale.z = radHei.x;
		ImGui::DragFloat3("Rotation", (float*)&Engine::Picking::selected->collider.rotation, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;

	case Engine::Collider::Shape::Box:
	case Engine::Collider::Shape::Mesh:
	{
		ImGui::DragFloat3("Scale", (float*)&Engine::Picking::selected->collider.scale, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Rotation", (float*)&Engine::Picking::selected->collider.rotation, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Position", (float*)&Engine::Picking::selected->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;
	}
}
void PropPhysics() {
	if (ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
			ImGui::Checkbox("Dynamic", &Engine::Picking::selected->dynamic);
			if (Engine::Picking::selected->dynamic) {
				ImGui::Checkbox("Gravity", &Engine::Picking::selected->gravity);
			}
			ImGui::SliderFloat("Friction", &Engine::Picking::selected->friction, 0.0f, 1.0f, "%.2f");
			if (Engine::Picking::selected->dynamic) {
				ImGui::DragFloat("Mass", &Engine::Picking::selected->mass, 0.1f, 0.0f, 1000000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::DragFloat3("Velocity", (float*)&Engine::Picking::selected->velocity, 0.1f, 0.0f, 0.0f, "%.2f");
				ImGui::DragFloat3("Angular", (float*)&Engine::Picking::selected->angular, 0.1f, 0.0f, 0.0f, "%.2f");
			}
			PropPhysCollider();
		}
		ImGui::End();
	}
}

void PropScripts() {
	if (ImGui::Begin("Scripts", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		if (Engine::Picking::selected) {
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
					Engine::Picking::selected->scripts.push_back(std::make_shared<Engine::Script>(filePath.string()));
				}
				ImGui::EndDragDropTarget();
			}

			// Script names
			for (int i = 0; i < Engine::Picking::selected->scripts.size(); i++) {
				// Draw
				ImGui::PushID(i);
				ImGui::Selectable(std::filesystem::path(Engine::Picking::selected->scripts[i]->path).stem().string().c_str());
				ImGui::PopID();

				// RMB menu
				if (ImGui::BeginPopupContextItem()) {
					// Delete
					if (!Engine::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f))) {
						Engine::Picking::selected->scripts.erase(Engine::Picking::selected->scripts.begin() + i);
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
