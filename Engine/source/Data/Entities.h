#pragma once

#include "KrimzLib.h"
#include "Data/EObjects.h"
#include "Data/Meshes.h"
#include "Data/Textures.h"
#include "Scripting/Script.h"
#include "Physics/Physics.h"
#include "Physics/Collider.h"


namespace Engine {
	class Entity : public EObject {
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

		// Mesh pointer
		Engine::Mesh* mesh = nullptr;

		// Texture pointer
		Engine::Texture* texture = nullptr;

		// Scripts
		std::vector<Engine::Script> scripts;

		Entity();
		Entity(const String& name, Engine::Mesh* mesh, Engine::Texture* texture);

		// Script callers
		void callStarts();
		void callUpdates();

		// Returns the world matrix
		kl::mat4 matrix() const;

		// Renders the mesh
		void render(kl::gpu* gpu, bool useTex) const;
	};

	inline std::list<Engine::Entity> entities;
	bool find(const std::list<Engine::Entity>& entities, const String& name);
}
