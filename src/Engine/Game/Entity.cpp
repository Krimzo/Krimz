#include "Engine/Game/Entity.h"
#include "Engine/Scripting/Scripting.h"


Engine::Game::Entity::Entity() {
	this->name = "undefined";
}
Engine::Game::Entity::Entity(const std::string& name, ID3D11Buffer* mes, ID3D11ShaderResourceView* tex) {
	this->name = name;
	mesh = mes;
	texture = tex;
}

// Script callers
void Engine::Game::Entity::callStarts() {
	for (Engine::Script* ref : scripts) {
		ref->callStart(this);
	}
}
void Engine::Game::Entity::callUpdates() {
	for (Engine::Script* ref : scripts) {
		ref->callUpdate(this);
	}
}

// Updates the object physics
void Engine::Game::Entity::upPhys(float deltaT) {
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
kl::mat4 Engine::Game::Entity::matrix() const {
	return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
}

// Renders the mesh
void Engine::Game::Entity::render(kl::gpu* gpu, bool useTex) const {
	// Binding the texture
	if (useTex) {
		gpu->bindPixlTex(texture, 0);
	}

	// Rendering
	gpu->draw(mesh);
}
