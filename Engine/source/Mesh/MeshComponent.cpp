#include "Mesh/MeshComponent.h"


Krimz::MeshComponent::MeshComponent() {

}

Krimz::MeshComponent::MeshComponent(kl::ref<Mesh> mesh) : mesh(mesh) {

}

void Krimz::MeshComponent::render(kl::gpu& gpu) {
	if (mesh) {
		gpu.draw(mesh->buffer);
	}
}

void Krimz::MeshComponent::gui_render() {
	if (mesh) {

	}
}
