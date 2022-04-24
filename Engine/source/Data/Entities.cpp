#include "Data/Entities.h"
#include "Scripting/Scripting.h"


void FixEntityName(String& name) {
	const String nameCopy = name;
	int counter = 0;
	while (Engine::find(Engine::entities, name)) {
		name = nameCopy + "_" + std::to_string(++counter);
	}
}

Engine::Entity::Entity() : EObject("undefined"), mesh(Engine::Default::cube), texture(Engine::Default::texture) {
	FixEntityName(this->name);
}
Engine::Entity::Entity(const String& name, Engine::Mesh* mesh, Engine::Texture* texture) : EObject(name), mesh(mesh), texture(texture) {
	FixEntityName(this->name);
}

// Script callers
void Engine::Entity::callStarts() {
	for (auto& ref : scripts) {
		ref.callStart(this);
	}
}
void Engine::Entity::callUpdates() {
	for (auto& ref : scripts) {
		ref.callUpdate(this);
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

// Checks the buffer for the name
bool Engine::find(const std::list<Engine::Entity>& entities, const String& name) {
	for (auto& ent : entities) {
		if (ent.name == name) {
			return true;
		}
	}
	return false;
}
