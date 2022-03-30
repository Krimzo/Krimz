#include "Game/Entity.h"
#include "Game/Game.h"
#include "Scripting/Scripting.h"
#include "Render/Meshes.h"
#include "Render/Textures.h"


void FixNameDuplicate(std::string& name) {
	const std::string nameCopy = name;
	int counter = 0;
	while ([&]() {
		for (int i = 0; i < Engine::entities.size(); i++) {
			if (Engine::entities[i]->name == name) {
				return true;
			}
		}
		return false;
		}()) {
		name = nameCopy + "_" + std::to_string(++counter);
	}
}

Engine::Entity::Entity() {
	this->name = "undefined";
	this->mesh = Engine::Default::mesh;
	this->texture = Engine::Default::texture;
	FixNameDuplicate(this->name);
}
Engine::Entity::Entity(const std::string& name, Engine::Mesh* mes, Engine::Texture* tex) {
	this->name = name;
	mesh = mes;
	texture = tex;
	FixNameDuplicate(this->name);
}

// Script callers
void Engine::Entity::callStarts() {
	for (Engine::Script* ref : scripts) {
		ref->callStart(this);
	}
}
void Engine::Entity::callUpdates() {
	for (Engine::Script* ref : scripts) {
		ref->callUpdate(this);
	}
}

// Updates the object physics
void Engine::Entity::upPhys(float deltaT) {
	if (physics) {
		// Applying acceleration
		velocity += acceler * deltaT;

		// Applying velocity
		position += velocity * deltaT;

		// Applying angular momentum
		rotation += angular * deltaT;
	}
}

// Returns the world matrix
kl::mat4 Engine::Entity::matrix() const {
	return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
}

// Renders the mesh
void Engine::Entity::render(kl::gpu* gpu, bool useTex) const {
	// Binding the texture
	if (useTex) {
		gpu->bindPixlTex(texture->view, 0);
	}

	// Rendering
	gpu->draw(mesh->buff);
}
