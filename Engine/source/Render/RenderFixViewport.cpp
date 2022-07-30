#include "Render/Render.h"
#include "Types/Camera.h"
#include "Editor/Picking.h"
#include "GUI/GUI.h"


void Krimz::Render::FixViewport() {
	if (Krimz::Render::targetSize != Krimz::GUI::viewportSize) {
		Krimz::Render::targetSize = Krimz::GUI::viewportSize;

		Krimz::gpu->destroy(Krimz::Render::targetView);
		Krimz::gpu->destroy(Krimz::Render::shaderView);

		kl::dx::desc::texture viewportDescriptor = {};
		viewportDescriptor.Width = Krimz::Render::targetSize.x;
		viewportDescriptor.Height = Krimz::Render::targetSize.y;
		viewportDescriptor.MipLevels = 1;
		viewportDescriptor.ArraySize = 1;
		viewportDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		viewportDescriptor.SampleDesc.Count = 1;
		viewportDescriptor.Usage = D3D11_USAGE_DEFAULT;
		viewportDescriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		kl::dx::texture viewportTexture = Krimz::gpu->newTexture(&viewportDescriptor);
		Krimz::Render::targetView = Krimz::gpu->newTargetView(viewportTexture);
		Krimz::Render::shaderView = Krimz::gpu->newShaderView(viewportTexture);
		Krimz::gpu->destroy(viewportTexture);

		Krimz::gpu->destroy(Krimz::Picking::texture);
		Krimz::gpu->destroy(Krimz::Picking::textureStaging);
		Krimz::gpu->destroy(Krimz::Picking::targetView);
		viewportDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
		Krimz::Picking::texture = Krimz::gpu->newTexture(&viewportDescriptor);
		Krimz::Picking::textureStaging = Krimz::gpu->newTextureST(Krimz::Picking::texture, 1);
		Krimz::Picking::targetView = Krimz::gpu->newTargetView(Krimz::Picking::texture);

		Krimz::gpu->destroy(Krimz::Outline::targetView);
		Krimz::gpu->destroy(Krimz::Outline::shaderView);
		viewportDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
		kl::dx::texture outlineTexture = Krimz::gpu->newTexture(&viewportDescriptor);
		Krimz::Outline::targetView = Krimz::gpu->newTargetView(outlineTexture);
		Krimz::Outline::shaderView = Krimz::gpu->newShaderView(outlineTexture);
		Krimz::gpu->destroy(outlineTexture);
	}

	Krimz::gpu->viewport(0, Krimz::Render::targetSize);
	Krimz::Selected::camera->aspect = float(Krimz::Render::targetSize.x) / Krimz::Render::targetSize.y;
}
