#pragma once

#include "KrimzLib.h"
#include "Types/Named.h"
#include "Types/Mesh.h"
#include "Types/Material.h"
#include "Scripting/Script.h"
#include "Physics/Physics.h"
#include "Physics/Collider.h"


namespace Engine {
	class Entity : public Named {
	public:
		bool visible = true;
		bool shadows = true;
		Engine::Material material;

		kl::float3 scale = kl::float3(1.0f);
		kl::float3 rotation;
		kl::float3 position;
		kl::reference<Engine::Mesh> mesh;

		bool dynamic = false;
		bool gravity = true;
		float friction = 0.5f;
		float mass = 10.0f;
		kl::float3 velocity;
		kl::float3 angular;
		Engine::Collider collider;

		std::vector<kl::reference<Engine::Script>> scripts;

		Entity();
		Entity(const std::string& name, const kl::reference<Engine::Mesh>& mesh, const Engine::Material& material);
		Entity(const Engine::Entity& obj);

		void callStarts();
		void callUpdates();

		kl::mat4 matrix() const;
		void render(bool useTex) const;
	};

	namespace Selected {
		inline kl::reference<Engine::Entity> entity;
	}

	inline std::list<kl::reference<Engine::Entity>> entities;
}
