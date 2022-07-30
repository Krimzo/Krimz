#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"


void Krimz::Render::Collider() {
	if (!Krimz::Colliders::shouldRender) {
		return;
	}

	Krimz::gpu->bindTargets({ Krimz::Render::targetView });
	Krimz::gpu->bind(Krimz::Rasters::wire);
	Krimz::gpu->bind(Krimz::DepthStencil::depth);
	Krimz::gpu->bind(Krimz::Shaders::collider);

	kl::mat4 wvp = Krimz::Selected::camera->matrix() *
		kl::mat4::translation(Krimz::Selected::entity->position + Krimz::Selected::entity->collider.position) *
		kl::mat4::rotation(Krimz::Selected::entity->rotation + Krimz::Selected::entity->collider.rotation);

	const kl::float4 colliderColor = Krimz::Colliders::color;
	Krimz::gpu->autoPixelCBuffer(colliderColor);

	switch (Krimz::Selected::entity->collider.shape) {
		case Krimz::Collider::Shape::Box:
		{
			wvp *= kl::mat4::scaling(Krimz::Selected::entity->scale * Krimz::Selected::entity->collider.scale);
			Krimz::gpu->autoVertexCBuffer(wvp);
			Krimz::gpu->draw(Krimz::Meshes::Default::cube->buffer);
		}
		break;

		case Krimz::Collider::Shape::Sphere:
		{
			wvp *= kl::mat4::scaling(Krimz::Selected::entity->collider.scale);
			Krimz::gpu->autoVertexCBuffer(wvp);
			Krimz::gpu->draw(Krimz::Meshes::Default::sphere->buffer);
		}
		break;

		case Krimz::Collider::Shape::Capsule:
		{
			wvp *= kl::mat4::scaling(Krimz::Selected::entity->collider.scale);
			Krimz::gpu->autoVertexCBuffer(wvp);
			Krimz::gpu->draw(Krimz::Meshes::Default::capsule->buffer);
		}
		break;

		case Krimz::Collider::Shape::Mesh:
		{
			wvp *= kl::mat4::scaling(Krimz::Selected::entity->scale * Krimz::Selected::entity->collider.scale);
			Krimz::gpu->autoVertexCBuffer(wvp);
			Krimz::gpu->draw(Krimz::Selected::entity->mesh->buffer);
		}
		break;
	}
}
