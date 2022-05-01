#include "Data/Entities.h"
#include "Scripting/Scripting.h"


Engine::Entity::Entity() : Named(Named::Type::Entity), mesh(Engine::Default::cube), texture(Engine::Default::texture) {}
Engine::Entity::Entity(const String& name, Engine::Mesh* mesh, Engine::Texture* texture) : Named(Named::Type::Entity, name), mesh(mesh), texture(texture) {}
Engine::Entity::Entity(const Engine::Entity& obj) : Named(Named::Type::Entity, obj.getName()) {
	// View
	visible = obj.visible;
	shadows = obj.shadows;
	roughness = obj.roughness;

	// Geometry
	scale = obj.scale;
	rotation = obj.rotation;
	position = obj.position;

	// Physics
	dynamic = obj.dynamic;
	gravity = obj.gravity;
	friction = obj.friction;
	mass = obj.mass;
	velocity = obj.velocity;
	angular = obj.angular;
	collider = obj.collider;

	// Mesh/texture
	mesh = obj.mesh;
	texture = obj.texture;

	// Scripts
	scripts = obj.scripts;
}

// Script callers
void Engine::Entity::callStarts() {
	for (auto& ref : scripts) {
		ref->callStart(this);
	}
}
void Engine::Entity::callUpdates() {
	for (auto& ref : scripts) {
		ref->callUpdate(this);
	}
}

// Returns the world matrix
kl::mat4 Engine::Entity::matrix() const {
	return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(scale);
}

// Renders the mesh
void Engine::Entity::render(kl::gpu* gpu, bool useTex) const {
	// Texture bind
	if (useTex) {
		gpu->bindPixlTex(texture->view, 0);
	}

	// Rendering
	gpu->draw(mesh->buff);
}
