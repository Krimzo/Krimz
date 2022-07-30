#include "Entity/Entity.h"


Krimz::Entity::Entity(const std::string& name) : Named(Named::Type::Entity, name) {

}

Krimz::Entity::~Entity() {

}

void Krimz::Entity::render(kl::gpu& gpu) {
	if (material) {
		material->render(gpu);
	}
	if (mesh) {
		mesh->render(gpu);
	}
}

void Krimz::Entity::gui_render() {
	if (transform) {
		transform->gui_render();
	}
	if (mesh) {
		mesh->gui_render();
	}
	if (material) {
		material->gui_render();
	}
	if (physics) {
		physics->gui_render();
	}
	for (auto& scriptComponent : scripts) {
		if (scriptComponent) {
			scriptComponent->gui_render();
		}
	}
}
