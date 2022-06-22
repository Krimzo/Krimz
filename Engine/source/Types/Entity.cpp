#include "Types/Entity.h"
#include "Render/Render.h"
#include "Scripting/Scripting.h"


Engine::Entity::Entity() : Named(Named::Type::Entity), mesh(Engine::Meshes::Default::cube) {}
Engine::Entity::Entity(const std::string& name, const std::shared_ptr<Engine::Mesh>& mesh, const Engine::Material& material) : Named(Named::Type::Entity, name), mesh(mesh), material(material) {}
Engine::Entity::Entity(const Engine::Entity& obj) : Named(Named::Type::Entity, obj.getName()), mesh(obj.mesh), material(obj.material), collider(obj.collider) {
	visible = obj.visible;
	shadows = obj.shadows;

	scale = obj.scale;
	rotation = obj.rotation;
	position = obj.position;

	dynamic = obj.dynamic;
	gravity = obj.gravity;
	friction = obj.friction;
	mass = obj.mass;
	velocity = obj.velocity;
	angular = obj.angular;

	scripts = obj.scripts;
}

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

kl::mat4 Engine::Entity::matrix() const {
	return kl::mat4::translation(position) * kl::mat4::rotation(rotation) * kl::mat4::scaling(scale);
}

void Engine::Entity::render(bool useTex) const {
	if (useTex) {
		Engine::gpu->bindPixelShaderView(material.colorMap->view, 0);
		Engine::gpu->bindPixelShaderView(material.normalMap->view, 1);
		Engine::gpu->bindPixelShaderView(material.roughnessMap->view, 2);
	}
	Engine::gpu->draw(mesh->buffer);
}
