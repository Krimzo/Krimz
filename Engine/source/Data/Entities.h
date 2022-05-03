#pragma once

#include "KrimzLib.h"
#include "Data/Named.h"
#include "Data/Meshes.h"
#include "Data/Textures.h"
#include "Scripting/Script.h"
#include "Physics/Physics.h"
#include "Physics/Collider.h"


namespace Engine {
	class Entity : public Named {
	public:
		// View
		bool visible = true;
		bool shadows = true;
		float roughness = 0.5f;

		// Geometry
		kl::float3 scale = kl::float3(1.0f);
		kl::float3 rotation;
		kl::float3 position;

		// Physics
		bool dynamic = false;
		bool gravity = true;
		float friction = 0.5f;
		float mass = 10.0f;
		kl::float3 velocity;
		kl::float3 angular;
		Engine::Collider collider;

		// Mesh/texture
		std::shared_ptr<Engine::Mesh> mesh;
		std::shared_ptr<Engine::Texture> texture;

		// Scripts
		std::vector<std::shared_ptr<Engine::Script>> scripts;

		Entity();
		Entity(const String& name, const std::shared_ptr<Engine::Mesh>& mesh, const std::shared_ptr<Engine::Texture>& texture);
		Entity(const Engine::Entity& obj);

		// Script callers
		void callStarts();
		void callUpdates();

		// Returns the world matrix
		kl::mat4 matrix() const;

		// Renders the mesh
		void render(bool useTex) const;
	};

	namespace Selected {
		inline std::shared_ptr<Engine::Entity> entity;
	}

	inline std::list<std::shared_ptr<Engine::Entity>> entities;
}
