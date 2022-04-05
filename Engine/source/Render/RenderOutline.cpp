#include "Render/Render.h"
#include "Input/Picking.h"
#include "View/Outline.h"


void Engine::Render::Outline()
{
	// Screen mesh
	static ID3D11Buffer* screen = Engine::Render::gpu->newVertBuffer({
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});

	// Binding index shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::index);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::index);
	Engine::Render::gpu->bindVertCBuff(Engine::CBuffers::Vertex::index, 0);

	// Setting the index vertex data
	kl::mat4 wvp = Engine::Render::camera.matrix() * Engine::Picking::selected->matrix();
	Engine::Render::gpu->setBuffData(Engine::CBuffers::Vertex::index, &wvp);

	// Binding and clearing the outline buffer
	Engine::Render::gpu->bindTargets({ Engine::Outline::targetV });
	Engine::Render::gpu->clear(Engine::Outline::targetV, kl::float4(-1.0f));

	// Drawing the selected
	Engine::Picking::selected->render(Engine::Render::gpu, false);

	// Render target reset
	Engine::Render::gpu->bindInternal();

	// Binding outline shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::outline);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::outline);
	Engine::Render::gpu->bindPixlCBuff(Engine::CBuffers::Pixel::outline, 0);

	// Setting the outline pixel data
	kl::float4 higCol = Engine::Outline::color;
	Engine::Render::gpu->setBuffData(Engine::CBuffers::Pixel::outline, &higCol);

	// Binding the outline index tex
	Engine::Render::gpu->bindPixlTex(Engine::Outline::shaderV, 0);

	// Drawing the outline
	Engine::Render::gpu->bind(Engine::DepthStencil::mask);
	Engine::Render::gpu->draw(screen);
	Engine::Render::gpu->bind(Engine::DepthStencil::depth);
}
