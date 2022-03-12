#include "Engine/Engine.h"


void Resize(const kl::int2& siz) {
	if (siz.x > 0 && siz.y > 0) {
		// Internal
		gpu->regenBuffers(siz);
		gpu->viewport(kl::int2(0, 0), siz);
		camera.aspect = float(siz.x) / siz.y;

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
		gpu->destroy(pickingTex);
		gpu->destroy(pickingTexST);
		gpu->destroy(pickingTargetV);
		pickingTex = gpu->newTexture(&pickTexDes);
		pickingTexST = gpu->newTextureST(pickingTex, kl::int2(1));
		pickingTargetV = gpu->newTargetView(pickingTex);

		// Outline
		ID3D11Texture2D* outlineTex = gpu->newTexture(&pickTexDes);
		gpu->destroy(outlineTargetV);
		gpu->destroy(outlineShaderV);
		outlineTargetV = gpu->newTargetView(outlineTex);
		outlineShaderV = gpu->newShaderView(outlineTex);
		gpu->destroy(outlineTex);
	}
};
