#include "Render/Render.h"
#include "Types/Camera.h"
#include "Editor/Picking.h"
#include "GUI/GUI.h"


void Engine::Render::FixViewport() {
	if (Engine::Render::targetSize != Engine::GUI::viewportSize) {
		Engine::Render::targetSize = Engine::GUI::viewportSize;

		Engine::gpu->destroy(Engine::Render::targetView);
		Engine::gpu->destroy(Engine::Render::shaderView);

		kl::dx::desc::texture viewportDescriptor = {};
		viewportDescriptor.Width = Engine::Render::targetSize.x;
		viewportDescriptor.Height = Engine::Render::targetSize.y;
		viewportDescriptor.MipLevels = 1;
		viewportDescriptor.ArraySize = 1;
		viewportDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		viewportDescriptor.SampleDesc.Count = 1;
		viewportDescriptor.Usage = D3D11_USAGE_DEFAULT;
		viewportDescriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		kl::dx::texture viewportTexture = Engine::gpu->newTexture(&viewportDescriptor);
		Engine::Render::targetView = Engine::gpu->newTargetView(viewportTexture);
		Engine::Render::shaderView = Engine::gpu->newShaderView(viewportTexture);
		Engine::gpu->destroy(viewportTexture);

		Engine::gpu->destroy(Engine::Picking::texture);
		Engine::gpu->destroy(Engine::Picking::textureStaging);
		Engine::gpu->destroy(Engine::Picking::targetView);
		viewportDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
		Engine::Picking::texture = Engine::gpu->newTexture(&viewportDescriptor);
		Engine::Picking::textureStaging = Engine::gpu->newTextureST(Engine::Picking::texture, 1);
		Engine::Picking::targetView = Engine::gpu->newTargetView(Engine::Picking::texture);

		Engine::gpu->destroy(Engine::Outline::targetView);
		Engine::gpu->destroy(Engine::Outline::shaderView);
		viewportDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
		kl::dx::texture outlineTexture = Engine::gpu->newTexture(&viewportDescriptor);
		Engine::Outline::targetView = Engine::gpu->newTargetView(outlineTexture);
		Engine::Outline::shaderView = Engine::gpu->newShaderView(outlineTexture);
		Engine::gpu->destroy(outlineTexture);
	}

	Engine::gpu->viewport(0, Engine::Render::targetSize);
	Engine::Selected::camera->aspect = float(Engine::Render::targetSize.x) / Engine::Render::targetSize.y;
}
