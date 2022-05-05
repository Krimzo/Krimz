#include "Render/Render.h"
#include "Editor/Picking.h"


void Engine::Render::Collider() {
	// Render check
	if (!Engine::Outline::renderColliders) {
		return;
	}

	// Render target reset
	Engine::Render::gpu->bindTargets({ Engine::Render::targetV });

	// Raster bind
	Engine::Render::gpu->bind(Engine::Rasters::wire);

	// Depth bind
	Engine::Render::gpu->bind(Engine::DepthStencil::depth);

	// Binding collider shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::collider);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::collider);

	// wvp matrix
	kl::mat4 wvp = Engine::Selected::camera->matrix() *
		kl::mat4::translate(Engine::Selected::entity->position + Engine::Selected::entity->collider.position) *
		kl::mat4::rotate(Engine::Selected::entity->rotation + Engine::Selected::entity->collider.rotation);

	// Setting pxl data
	kl::float4 colliderColor = Engine::Outline::colliderColor;
	Engine::Render::gpu->autoSetPixlBuff(colliderColor);

	// Drawing the selected collider
	switch (Engine::Selected::entity->collider.shape) {
	case Engine::Collider::Shape::Box:
	{
		wvp *= kl::mat4::scale(Engine::Selected::entity->scale * Engine::Selected::entity->collider.scale);
		Engine::Render::gpu->autoSetVertBuff(wvp);
		Engine::Render::gpu->draw(Engine::Default::cube->buff);
	}
	break;

	case Engine::Collider::Shape::Sphere:
	{
		wvp *= kl::mat4::scale(Engine::Selected::entity->collider.scale);
		Engine::Render::gpu->autoSetVertBuff(wvp);
		Engine::Render::gpu->draw(Engine::Default::sphere->buff);
	}
	break;

	case Engine::Collider::Shape::Capsule:
	{
		wvp *= kl::mat4::scale(Engine::Selected::entity->collider.scale);
		Engine::Render::gpu->autoSetVertBuff(wvp);
		Engine::Render::gpu->draw(Engine::Default::capsule->buff);
	}
	break;

	case Engine::Collider::Shape::Mesh:
	{
		wvp *= kl::mat4::scale(Engine::Selected::entity->scale * Engine::Selected::entity->collider.scale);
		Engine::Render::gpu->autoSetVertBuff(wvp);
		Engine::Render::gpu->draw(Engine::Selected::entity->mesh->buff);
	}
	break;
	}
}
