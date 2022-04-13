#pragma once

#include "KrimzLib.h"
#include "Data/EObjects.h"
#include "Data/Meshes.h"
#include "Data/Textures.h"
#include "Scripting/Script.h"


namespace Engine
{
	class Entity : public EObject
	{
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
		bool physics = false;
		kl::float3 acceler;
		kl::float3 velocity;
		kl::float3 angular;

		// Mesh pointer
		Engine::Mesh* mesh = nullptr;

		// Texture pointer
		Engine::Texture* texture = nullptr;

		// Scripts
		std::vector<Engine::Script> scripts;

		// Constr
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

	inline kl::pbuffer<Engine::Entity> entities;
	bool find(const kl::pbuffer<Engine::Entity>& entities, const std::string& name);
}
