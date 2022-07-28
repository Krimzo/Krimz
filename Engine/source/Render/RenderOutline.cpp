#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"


void Krimz::Render::Outline()
{
	Krimz::gpu->bindTargets({ Krimz::Outline::targetView });
	Krimz::gpu->clear(Krimz::Outline::targetView, kl::float4(-1.0f));
	Krimz::gpu->bind(Krimz::Selected::raster);
	Krimz::gpu->bind(Krimz::DepthStencil::disabled);
	Krimz::gpu->bind(Krimz::Shaders::index);

	const kl::mat4 wvp = Krimz::Selected::camera->matrix() * Krimz::Selected::entity->matrix();
	Krimz::gpu->autoVertexCBuffer(wvp);
	Krimz::Selected::entity->render(false);

	Krimz::gpu->bindTargets({ Krimz::Render::targetView });
	Krimz::gpu->bind(Krimz::Rasters::solid);
	Krimz::gpu->bind(Krimz::Shaders::outline);

	const kl::float4 higCol = Krimz::Outline::color;
	Krimz::gpu->autoPixelCBuffer(higCol);
	Krimz::gpu->bindPixelShaderView(Krimz::Outline::shaderView, 0);
	Krimz::gpu->bind(Krimz::DepthStencil::mask);
	Krimz::gpu->draw(Krimz::Meshes::Util::screen->buffer);
	Krimz::gpu->bind(Krimz::DepthStencil::depth);
}
