#include "GUI/GUI.h"
#include "Editor/Picking.h"
#include "Scripting/Scripting.h"
#include "Stage/Stage.h"
#include "Physics/Physics.h"
#include "Render/Render.h"


void PropMeshPick(const kl::ref<Krimz::Mesh>& mesh)
{
	bool selectedOne = (Krimz::Selected::entity->mesh == mesh);
	if (ImGui::Selectable(mesh->getName().c_str(), selectedOne))
	{
		if (selectedOne)
		{
			ImGui::SetItemDefaultFocus();
		}
		else
		{
			Krimz::Selected::entity->mesh = mesh;
		}
	}
}
void PropGeometry()
{
	if (ImGui::Begin("Geometry", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Krimz::Selected::entity)
		{
			ImGui::DragFloat3("Scale", (float*) &Krimz::Selected::entity->scale, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat3("Rotation", (float*) &Krimz::Selected::entity->rotation, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat3("Position", (float*) &Krimz::Selected::entity->position, 0.1f, 0.0f, 0.0f, "%.2f");

			if (ImGui::BeginCombo("Mesh", Krimz::Selected::entity->mesh->getName().c_str()))
			{
				for (auto& mes : Krimz::meshes)
				{
					PropMeshPick(mes);
				}
				PropMeshPick(Krimz::Meshes::Default::cube);
				PropMeshPick(Krimz::Meshes::Default::sphere);
				PropMeshPick(Krimz::Meshes::Default::capsule);
				PropMeshPick(Krimz::Meshes::Default::pyramid);
				PropMeshPick(Krimz::Meshes::Default::monke);
				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}
}

void PropColorMapPick(const kl::ref<Krimz::Texture>& colorMap)
{
	bool selectedOne = (Krimz::Selected::entity->material.colorMap == colorMap);
	if (ImGui::Selectable(colorMap->getName().c_str(), selectedOne))
	{
		if (selectedOne)
		{
			ImGui::SetItemDefaultFocus();
		}
		else
		{
			Krimz::Selected::entity->material.colorMap = colorMap;
		}
	}
}
void PropNormalMapPick(const kl::ref<Krimz::Texture>& normalMap)
{
	bool selectedOne = (Krimz::Selected::entity->material.normalMap == normalMap);
	if (ImGui::Selectable(normalMap->getName().c_str(), selectedOne))
	{
		if (selectedOne)
		{
			ImGui::SetItemDefaultFocus();
		}
		else
		{
			Krimz::Selected::entity->material.normalMap = normalMap;
		}
	}
}
void PropRoughnessMapPick(const kl::ref<Krimz::Texture>& roughnessMap)
{
	bool selectedOne = (Krimz::Selected::entity->material.roughnessMap == roughnessMap);
	if (ImGui::Selectable(roughnessMap->getName().c_str(), selectedOne))
	{
		if (selectedOne)
		{
			ImGui::SetItemDefaultFocus();
		}
		else
		{
			Krimz::Selected::entity->material.roughnessMap = roughnessMap;
		}
	}
}
void PropView()
{
	if (ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Krimz::Selected::entity)
		{
			ImGui::Checkbox("Visible", &Krimz::Selected::entity->visible);
			ImGui::Checkbox("Shadows", &Krimz::Selected::entity->shadows);
			if (ImGui::BeginCombo("Color Map", Krimz::Selected::entity->material.colorMap->getName().c_str()))
			{
				for (auto& tex : Krimz::textures)
				{
					PropColorMapPick(tex);
				}
				PropColorMapPick(Krimz::Textures::Default::colorMap);
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Normal Map", Krimz::Selected::entity->material.normalMap->getName().c_str()))
			{
				for (auto& tex : Krimz::textures)
				{
					PropNormalMapPick(tex);
				}
				PropNormalMapPick(Krimz::Textures::Default::nullMap);
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Roughness Map", Krimz::Selected::entity->material.roughnessMap->getName().c_str()))
			{
				for (auto& tex : Krimz::textures)
				{
					PropRoughnessMapPick(tex);
				}
				PropRoughnessMapPick(Krimz::Textures::Default::nullMap);
				ImGui::EndCombo();
			}
			if (!Krimz::Selected::entity->material.hasRoughnessMap())
			{
				ImGui::SliderFloat("Roughness", &Krimz::Selected::entity->material.roughness, 0.0f, 1.0f, "%.2f");
			}
		}
		ImGui::End();
	}
}

void PropPhysCollider()
{
	if (ImGui::BeginCombo("Collider", Krimz::Collider::getName(Krimz::Selected::entity->collider.shape).c_str()))
	{
		if (ImGui::Selectable("None", Krimz::Selected::entity->collider.shape == Krimz::Collider::Shape::None))
		{
			Krimz::Selected::entity->collider.shape = Krimz::Collider::Shape::None;
		}
		if (ImGui::Selectable("Box", Krimz::Selected::entity->collider.shape == Krimz::Collider::Shape::Box))
		{
			Krimz::Selected::entity->collider.shape = Krimz::Collider::Shape::Box;
		}
		if (ImGui::Selectable("Sphere", Krimz::Selected::entity->collider.shape == Krimz::Collider::Shape::Sphere))
		{
			Krimz::Selected::entity->collider.shape = Krimz::Collider::Shape::Sphere;
		}
		if (ImGui::Selectable("Capsule", Krimz::Selected::entity->collider.shape == Krimz::Collider::Shape::Capsule))
		{
			Krimz::Selected::entity->collider.shape = Krimz::Collider::Shape::Capsule;
		}
		if (ImGui::Selectable("Mesh", Krimz::Selected::entity->collider.shape == Krimz::Collider::Shape::Mesh))
		{
			Krimz::Selected::entity->collider.shape = Krimz::Collider::Shape::Mesh;
		}
		ImGui::EndCombo();
	}
	switch (Krimz::Selected::entity->collider.shape)
	{
	case Krimz::Collider::Shape::Sphere:
	{
		ImGui::DragFloat("Radius", (float*) &Krimz::Selected::entity->collider.scale, 0.1f, 0.0f, 0.0f, "%.2f");
		Krimz::Selected::entity->collider.scale = kl::float3(Krimz::Selected::entity->collider.scale.x);
		ImGui::DragFloat3("Position", (float*) &Krimz::Selected::entity->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;

	case Krimz::Collider::Shape::Capsule:
	{
		kl::float2 radHei(Krimz::Selected::entity->collider.scale.y, Krimz::Selected::entity->collider.scale.x);
		ImGui::DragFloat2("Radius/Height", (float*) &radHei, 0.1f, 0.0f, 0.0f, "%.2f");
		Krimz::Selected::entity->collider.scale.x = radHei.y;
		Krimz::Selected::entity->collider.scale.y = radHei.x;
		Krimz::Selected::entity->collider.scale.z = radHei.x;
		ImGui::DragFloat3("Rotation", (float*) &Krimz::Selected::entity->collider.rotation, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Position", (float*) &Krimz::Selected::entity->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;

	case Krimz::Collider::Shape::Box:
	case Krimz::Collider::Shape::Mesh:
	{
		ImGui::DragFloat3("Scale", (float*) &Krimz::Selected::entity->collider.scale, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Rotation", (float*) &Krimz::Selected::entity->collider.rotation, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Position", (float*) &Krimz::Selected::entity->collider.position, 0.1f, 0.0f, 0.0f, "%.2f");
	}
	break;
	}
}
void PropPhysics()
{
	if (ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Krimz::Selected::entity)
		{
			ImGui::Checkbox("Dynamic", &Krimz::Selected::entity->dynamic);
			if (Krimz::Selected::entity->dynamic)
			{
				ImGui::Checkbox("Gravity", &Krimz::Selected::entity->gravity);
			}
			ImGui::SliderFloat("Friction", &Krimz::Selected::entity->friction, 0.0f, 1.0f, "%.2f");
			if (Krimz::Selected::entity->dynamic)
			{
				ImGui::DragFloat("Mass", &Krimz::Selected::entity->mass, 0.1f, 0.0f, 1000000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::DragFloat3("Velocity", (float*) &Krimz::Selected::entity->velocity, 0.1f, 0.0f, 0.0f, "%.2f");
				ImGui::DragFloat3("Angular", (float*) &Krimz::Selected::entity->angular, 0.1f, 0.0f, 0.0f, "%.2f");
			}
			PropPhysCollider();
		}
		ImGui::End();
	}
}

void PropScripts()
{
	if (ImGui::Begin("Scripts", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (Krimz::Selected::entity)
		{
			ImVec2 winPos = ImGui::GetWindowPos();
			ImVec2 winSize = ImGui::GetWindowSize();
			ImGuiID winId = ImGui::GetID("Scripts");
			if (ImGui::BeginDragDropTargetCustom(ImRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y)), winId))
			{
				if (ImGui::GetDragDropPayload()->IsDataType("ScriptTransfer"))
				{
					ImGui::GetForegroundDrawList()->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), IM_COL32(180, 100, 0, 255));
				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ScriptTransfer"))
				{
					std::filesystem::path filePath((char*) payload->Data);
					Krimz::Selected::entity->scripts.push_back(kl::make<Krimz::Script>(filePath.string()));
				}
				ImGui::EndDragDropTarget();
			}

			for (int i = 0; i < Krimz::Selected::entity->scripts.size(); i++)
			{
				ImGui::PushID(i);
				ImGui::Selectable(std::filesystem::path(Krimz::Selected::entity->scripts[i]->path).stem().string().c_str());
				ImGui::PopID();

				if (ImGui::BeginPopupContextItem())
				{
					if (!Krimz::gameRunning && ImGui::Button("Delete", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
					{
						Krimz::Selected::entity->scripts.erase(Krimz::Selected::entity->scripts.begin() + i);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
		}

		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::Button("Reload", ImVec2(ImGui::GetWindowContentRegionWidth(), 0.0f)))
			{
				Krimz::JavaHandler::ReloadScripts();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}

void Krimz::GUI::PropertiesRender()
{
	PropGeometry();
	PropView();
	PropPhysics();
	PropScripts();
}
