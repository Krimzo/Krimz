#pragma once

#include "KrimzLib.h"
#include "Scripting/Script.h"
#include "Render/Meshes.h"
#include "Render/Textures.h"


namespace Engine {
	class Entity {
	public:
		// Name
		std::string name;

		// View
		bool visible = true;
		bool shadows = true;
		float roughness = 0.5f;

		// Geometry
		kl::float3 size = kl::float3(1);
		kl::float3 rotation;
		kl::float3 position;

		// Physics
		bool physics = false;
		kl::float3 acceler;
		kl::float3 velocity;
		kl::float3 angular;

		// Mesh pointer
		Engine::Mesh* mesh = nullptr;

		// Texture pointer
		Engine::Texture* texture = nullptr;

		// Scripts
		std::vector<Engine::Script*> scripts;

		// Constructor
		Entity();
		Entity(const std::string& name, Engine::Mesh* mes, Engine::Texture* tex);

		// Script callers
		void callStarts();
		void callUpdates();

		// Updates the object physics
		void upPhys(float deltaT);

		// Returns the world matrix
		kl::mat4 matrix() const;

		// Renders the mesh
		void render(kl::gpu* gpu, bool useTex) const;
	};
}
