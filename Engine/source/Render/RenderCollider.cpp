#include "Render/Render.h"
#include "Input/Picking.h"


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
	Engine::Render::gpu->bindVertCBuff(Engine::CBuffers::buff64_1, 0);
	Engine::Render::gpu->bindPixlCBuff(Engine::CBuffers::buff16_1, 0);

	// wvp matrix
	kl::mat4 wvp = Engine::Render::camera.matrix() *
		kl::mat4::translate(Engine::Picking::selected->position + Engine::Picking::selected->collider.position) *
		kl::mat4::rotate(Engine::Picking::selected->rotation + Engine::Picking::selected->collider.rotation);

	// Setting pxl data
	kl::float4 colliderColor = Engine::Outline::colliderColor;
	Engine::Render::gpu->setBuffData(Engine::CBuffers::buff16_1, &colliderColor);

	// Drawing the selected collider
	switch (Engine::Picking::selected->collider.shape) {
	case Engine::Collider::Shape::Box:
	{
		wvp *= kl::mat4::scale(Engine::Picking::selected->scale * Engine::Picking::selected->collider.scale);
		Engine::Render::gpu->setBuffData(Engine::CBuffers::buff64_1, &wvp);
		Engine::Render::gpu->draw(Engine::Default::cube->buff);
	}
	break;

	case Engine::Collider::Shape::Sphere:
	{
		wvp *= kl::mat4::scale(Engine::Picking::selected->collider.scale);
		Engine::Render::gpu->setBuffData(Engine::CBuffers::buff64_1, &wvp);
		Engine::Render::gpu->draw(Engine::Default::sphere->buff);
	}
	break;

	case Engine::Collider::Shape::Capsule:
	{
		wvp *= kl::mat4::scale(Engine::Picking::selected->collider.scale);
		Engine::Render::gpu->setBuffData(Engine::CBuffers::buff64_1, &wvp);
		Engine::Render::gpu->draw(Engine::Default::capsule->buff);
	}
	break;

	case Engine::Collider::Shape::Mesh:
	{
		wvp *= kl::mat4::scale(Engine::Picking::selected->scale * Engine::Picking::selected->collider.scale);
		Engine::Render::gpu->setBuffData(Engine::CBuffers::buff64_1, &wvp);
		Engine::Render::gpu->draw(Engine::Picking::selected->mesh->buff);
	}
	break;
	}
}
