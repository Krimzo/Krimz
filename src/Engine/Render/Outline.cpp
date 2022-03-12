#include "Engine/Engine.h"


void Outline() {
	// Screen mesh
	static ID3D11Buffer* screen = gpu->newVertBuffer({
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
	});

	// Binding index shaders
	gpu->bind(index_vtx);
	gpu->bind(index_pxl);
	gpu->bindVertCBuff(index_vtx_cb, 0);

	// Setting the index vertex data
	kl::mat4 wvp = camera.matrix() * selected->matrix();
	gpu->setBuffData(index_vtx_cb, &wvp);

	// Clearing the outline index target
	gpu->clear(outlineTargetV, kl::float4(-1.0f));

	// Drawing the selected
	gpu->bindTargets({ outlineTargetV });
	selected->render(gpu, false);

	// Render target reset
	gpu->bindInternal();

	// Binding outline shaders
	gpu->bind(outline_vtx);
	gpu->bind(outline_pxl);
	gpu->bindPixlCBuff(outline_pxl_cb, 0);

	// Setting the outline pixel data
	kl::float4 higCol = outline;
	gpu->setBuffData(outline_pxl_cb, &higCol);

	// Binding the outline index tex
	gpu->bindPixlTex(outlineShaderV, 0);

	// Drawing the outline
	gpu->bind(mask_ds);
	gpu->draw(screen);
	gpu->bind(depth_ds);
}
