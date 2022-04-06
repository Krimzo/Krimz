#include "Data/Entities.h"
#include "Scripting/Scripting.h"


void FixEntityName(std::string& name)
{
	const std::string nameCopy = name;
	int counter = 0;
	while (Engine::find(Engine::entities, name))
		name = nameCopy + "_" + std::to_string(++counter);
}

Engine::Entity::Entity() : EObject::EObject("undefined")
{
	this->mesh = Engine::Default::mesh;
	this->texture = Engine::Default::texture;
	FixEntityName(this->name);
}
Engine::Entity::Entity(const std::string& name, Engine::Mesh* mes, Engine::Texture* tex) : EObject::EObject(name)
{
	mesh = mes;
	texture = tex;
	FixEntityName(this->name);
}

// Script callers
void Engine::Entity::callStarts()
{
	for (auto& ref : scripts)
		ref.callStart(this);
}
void Engine::Entity::callUpdates()
{
	for (auto& ref : scripts)
		ref.callUpdate(this);
}

// Updates the object physics
void Engine::Entity::upPhys(float deltaT)
{
	if (physics)
	{
		// Applying acceleration
		velocity += acceler * deltaT;

		// Applying velocity
		position += velocity * deltaT;

		// Applying angular momentum
		rotation += angular * deltaT;
	}
}

// Returns the world matrix
kl::mat4 Engine::Entity::matrix() const
{
	return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(scale);
}

// Renders the mesh
void Engine::Entity::render(kl::gpu* gpu, bool useTex) const
{
	// Binding the texture
	if (useTex)
		gpu->bindPixlTex(texture->view, 0);

	// Rendering
	gpu->draw(mesh->buff);
}

// Checks the buffer for the name
bool Engine::find(const kl::pbuffer<Engine::Entity>& entities, const std::string& name)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->name == name)
			return true;
	}
	return false;
}
