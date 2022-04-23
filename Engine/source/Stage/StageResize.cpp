#include "Stage/Stage.h"
#include "Window/Window.h"
#include "Render/Render.h"
#include "Input/Picking.h"


void Engine::Stage::Resize(const kl::int2& siz) {
	if (siz.x > 0 && siz.y > 0) {
		// Internal
		Engine::Render::gpu->regenBuffers(siz);
		Engine::Render::gpu->viewport(kl::int2(0, 0), siz);
		Engine::Render::camera.aspect = float(siz.x) / siz.y;

		// Picking
		D3D11_TEXTURE2D_DESC pickTexDes = {};
		pickTexDes.Width = siz.x;
		pickTexDes.Height = siz.y;
		pickTexDes.MipLevels = 1;
		pickTexDes.ArraySize = 1;
		pickTexDes.Format = DXGI_FORMAT_R32_FLOAT;
		pickTexDes.SampleDesc.Count = 1;
		pickTexDes.Usage = D3D11_USAGE_DEFAULT;
		pickTexDes.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Engine::Render::gpu->destroy(Engine::Picking::texture);
		Engine::Render::gpu->destroy(Engine::Picking::textureStag);
		Engine::Render::gpu->destroy(Engine::Picking::targetV);
		Engine::Picking::texture = Engine::Render::gpu->newTexture(&pickTexDes);
		Engine::Picking::textureStag = Engine::Render::gpu->newTextureST(Engine::Picking::texture, kl::int2(1));
		Engine::Picking::targetV = Engine::Render::gpu->newTargetView(Engine::Picking::texture);

		// Outline
		ID3D11Texture2D* outlineTex = Engine::Render::gpu->newTexture(&pickTexDes);
		Engine::Render::gpu->destroy(Engine::Render::targetV);
		Engine::Render::gpu->destroy(Engine::Render::shaderV);
		Engine::Render::targetV = Engine::Render::gpu->newTargetView(outlineTex);
		Engine::Render::shaderV = Engine::Render::gpu->newShaderView(outlineTex);
		Engine::Render::gpu->destroy(outlineTex);
	}
};
