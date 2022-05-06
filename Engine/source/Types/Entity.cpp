#include "Types/Entity.h"
#include "Render/Render.h"
#include "Scripting/Scripting.h"


Engine::Entity::Entity() : Named(Named::Type::Entity), mesh(Engine::Default::cube) {}
Engine::Entity::Entity(const String& name, const std::shared_ptr<Engine::Mesh>& mesh, const Engine::Material& material) : Named(Named::Type::Entity, name), mesh(mesh), material(material) {}
Engine::Entity::Entity(const Engine::Entity& obj) : Named(Named::Type::Entity, obj.getName()), mesh(obj.mesh), material(obj.material), collider(obj.collider) {
	// View
	visible = obj.visible;
	shadows = obj.shadows;

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
void Engine::Entity::render(bool useTex) const {
	// Texture bind
	if (useTex) {
		Engine::Render::gpu->bindPixlTex(material.colorMap->view, 0);
		Engine::Render::gpu->bindPixlTex(material.normalMap->view, 1);
		Engine::Render::gpu->bindPixlTex(material.roughnessMap->view, 2);
	}

	// Rendering
	Engine::Render::gpu->draw(mesh->buff);
}
