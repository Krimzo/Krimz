#include "Render/Render.h"
#include "Types/Camera.h"
#include "Editor/Picking.h"
#include "GUI/GUI.h"


void Engine::Render::FixViewport() {
	if (Engine::Render::targetSize != Engine::GUI::viewportSize) {
		// Size save
		Engine::Render::targetSize = Engine::GUI::viewportSize;

		// Render texture
		Engine::Render::gpu->destroy(Engine::Render::targetV);
		Engine::Render::gpu->destroy(Engine::Render::shaderV);
		D3D11_TEXTURE2D_DESC vpTexDes = {};
		vpTexDes.Width = Engine::Render::targetSize.x;
		vpTexDes.Height = Engine::Render::targetSize.y;
		vpTexDes.MipLevels = 1;
		vpTexDes.ArraySize = 1;
		vpTexDes.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		vpTexDes.SampleDesc.Count = 1;
		vpTexDes.Usage = D3D11_USAGE_DEFAULT;
		vpTexDes.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		ID3D11Texture2D* vpTex = Engine::Render::gpu->newTexture(&vpTexDes);
		Engine::Render::targetV = Engine::Render::gpu->newTargetView(vpTex);
		Engine::Render::shaderV = Engine::Render::gpu->newShaderView(vpTex);
		Engine::Render::gpu->destroy(vpTex);

		// Picking texture
		Engine::Render::gpu->destroy(Engine::Picking::texture);
		Engine::Render::gpu->destroy(Engine::Picking::textureStag);
		Engine::Render::gpu->destroy(Engine::Picking::targetV);
		vpTexDes.Format = DXGI_FORMAT_R32_FLOAT;
		Engine::Picking::texture = Engine::Render::gpu->newTexture(&vpTexDes);
		Engine::Picking::textureStag = Engine::Render::gpu->newTextureST(Engine::Picking::texture, kl::int2(1));
		Engine::Picking::targetV = Engine::Render::gpu->newTargetView(Engine::Picking::texture);

		// Outline texture
		Engine::Render::gpu->destroy(Engine::Outline::targetV);
		Engine::Render::gpu->destroy(Engine::Outline::shaderV);
		vpTexDes.Format = DXGI_FORMAT_R32_FLOAT;
		ID3D11Texture2D* outlineTex = Engine::Render::gpu->newTexture(&vpTexDes);
		Engine::Outline::targetV = Engine::Render::gpu->newTargetView(outlineTex);
		Engine::Outline::shaderV = Engine::Render::gpu->newShaderView(outlineTex);
		Engine::Render::gpu->destroy(outlineTex);
	}

	// GPU viewport fix
	Engine::Render::gpu->viewport(kl::int2(0), Engine::Render::targetSize);
	Engine::Selected::camera->aspect = float(Engine::Render::targetSize.x) / Engine::Render::targetSize.y;
}
