#pragma once

#include "Named/Named.h"
#include "Transform/TransformComponent.h"
#include "Mesh/MeshComponent.h"
#include "Material/MaterialComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Scripter/ScriptComponent.h"


namespace Krimz
{
	class Entity : public Named, public Renderable, public GUIRenderable
	{
	public:
		kl::ref<TransformComponent> transform;
		kl::ref<MeshComponent> mesh;
		kl::ref<MaterialComponent> material;
		kl::ref<PhysicsComponent> physics;
		std::vector<kl::ref<ScriptComponent>> scripts;

		Entity(const std::string& name = "undefined");
		Entity(const Entity&) = delete;
		void operator=(const Entity&) = delete;
		~Entity();

		void render(kl::gpu& gpu) override;
		void gui_render() override;
	};
}
