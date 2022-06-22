#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"


void Engine::Render::Collider() {
	if (!Engine::Colliders::shouldRender) {
		return;
	}

	Engine::gpu->bindTargets({ Engine::Render::targetView });
	Engine::gpu->bind(Engine::Rasters::wire);
	Engine::gpu->bind(Engine::DepthStencil::depth);
	Engine::gpu->bind(Engine::Shaders::collider);

	kl::mat4 wvp = Engine::Selected::camera->matrix() *
		kl::mat4::translation(Engine::Selected::entity->position + Engine::Selected::entity->collider.position) *
		kl::mat4::rotation(Engine::Selected::entity->rotation + Engine::Selected::entity->collider.rotation);

	const kl::float4 colliderColor = Engine::Colliders::color;
	Engine::gpu->autoPixelCBuffer(colliderColor);

	switch (Engine::Selected::entity->collider.shape) {
	case Engine::Collider::Shape::Box:
	{
		wvp *= kl::mat4::scaling(Engine::Selected::entity->scale * Engine::Selected::entity->collider.scale);
		Engine::gpu->autoVertexCBuffer(wvp);
		Engine::gpu->draw(Engine::Meshes::Default::cube->buffer);
	}
	break;

	case Engine::Collider::Shape::Sphere:
	{
		wvp *= kl::mat4::scaling(Engine::Selected::entity->collider.scale);
		Engine::gpu->autoVertexCBuffer(wvp);
		Engine::gpu->draw(Engine::Meshes::Default::sphere->buffer);
	}
	break;

	case Engine::Collider::Shape::Capsule:
	{
		wvp *= kl::mat4::scaling(Engine::Selected::entity->collider.scale);
		Engine::gpu->autoVertexCBuffer(wvp);
		Engine::gpu->draw(Engine::Meshes::Default::capsule->buffer);
	}
	break;

	case Engine::Collider::Shape::Mesh:
	{
		wvp *= kl::mat4::scaling(Engine::Selected::entity->scale * Engine::Selected::entity->collider.scale);
		Engine::gpu->autoVertexCBuffer(wvp);
		Engine::gpu->draw(Engine::Selected::entity->mesh->buffer);
	}
	break;
	}
}
