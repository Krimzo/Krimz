#include "Types/Light/Direct.h"
#include "Render/Render.h"


Engine::Light::Direct::Direct(const kl::int2& size) : texSize(size) {
	// Texture gen
	D3D11_TEXTURE2D_DESC depTexDesc = {};
	depTexDesc.Width = UINT(size.x);
	depTexDesc.Height = UINT(size.y);
	depTexDesc.MipLevels = 1;
	depTexDesc.ArraySize = 1;
	depTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depTexDesc.SampleDesc.Count = 1;
	depTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* depTexs[4] = {};
	for (auto& depTex : depTexs) {
		depTex = Engine::Render::gpu->newTexture(&depTexDesc);
	}

	// Depth view gen
	D3D11_DEPTH_STENCIL_VIEW_DESC depthVDesc = {};
	depthVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	for (int i = 0; i < 4; i++) {
		depthVs[i] = Engine::Render::gpu->newDepthView(depTexs[i], &depthVDesc);
	}

	// Shader view gen
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderVDesc = {};
	shaderVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderVDesc.Texture2D.MipLevels = 1;
	for (int i = 0; i < 4; i++) {
		shaderVs[i] = Engine::Render::gpu->newShaderView(depTexs[i], &shaderVDesc);
	}

	// Cleanup
	for (auto& depTex : depTexs) {
		Engine::Render::gpu->destroy(depTex);
	}
}
Engine::Light::Direct::~Direct() {
	if (Engine::Render::gpu) {
		for (auto& depthV : depthVs) {
			Engine::Render::gpu->destroy(depthV);
			depthV = nullptr;
		}
		for (auto& shaderV : shaderVs) {
			Engine::Render::gpu->destroy(shaderV);
			shaderV = nullptr;
		}
	}
}

kl::int2 Engine::Light::Direct::getSize() const {
	return texSize;
}

kl::float3 Engine::Light::Direct::getDir() const {
	return direction;
}
void Engine::Light::Direct::setDir(const kl::float3& dir) {
	direction = dir.normalize();
}

kl::mat4 Engine::Light::Direct::matrix(const kl::camera& cam, uint32_t ind) const {
	// Ind check
	if (ind >= 4) {
		return {};
	}

	// Inverse camera matrix calculation
	kl::camera copyCam = cam;
	copyCam.near = cam.near + (cam.far - cam.near) * (ind > 0 ? farLimits[ind - 1] : 0.0f);
	copyCam.far = cam.near + (cam.far - cam.near) * farLimits[ind];
	const kl::mat4 invCam = copyCam.matrix().inverse();

	// Frustum world corners
	std::vector<kl::float4> frustumCorners;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				const kl::float4 corner = invCam * kl::float4(x * 2.0f - 1.0f, y * 2.0f - 1.0f, z * 2.0f - 1.0f, 1.0f);
				frustumCorners.push_back(corner / corner.w);
			}
		}
	}

	// Centroid calculation
	kl::float3 centroid;
	for (auto& corn : frustumCorners) {
		centroid += corn.xyz();
	}
	centroid /= 8.0f;

	// Light view matrix
	const kl::mat4 view = kl::mat4::lookAt(centroid - getDir(), centroid, kl::float3::pos_y);

	// Finding min and max points
	kl::float3 minp(FLT_MAX);
	kl::float3 maxp(-FLT_MAX);
	for (auto& corn : frustumCorners) {
		const kl::float4 lightCorn = view * corn;
		minp.x = min(minp.x, lightCorn.x);
		maxp.x = max(maxp.x, lightCorn.x);
		minp.y = min(minp.y, lightCorn.y);
		maxp.y = max(maxp.y, lightCorn.y);
		minp.z = min(minp.z, lightCorn.z);
		maxp.z = max(maxp.z, lightCorn.z);
	}

	// Tunning z
	maxp.z *= 5.0f;

	// Light proj matrix
	const kl::mat4 proj = kl::mat4::ortho(minp.x, maxp.x, minp.y, maxp.y, maxp.z, minp.z);

	// Return
	return proj * view;
}

ID3D11DepthStencilView* Engine::Light::Direct::getDepthV(uint32_t ind) const {
	if (ind < 4) {
		return depthVs[ind];
	}
	return nullptr;
}
ID3D11ShaderResourceView* Engine::Light::Direct::getShaderV(uint32_t ind) const {
	if (ind < 4) {
		return shaderVs[ind];
	}
	return nullptr;
}

kl::float4 Engine::Light::Direct::getFBounds(const kl::camera& cam) const {
	kl::float4 bounds;
	bounds.x = cam.near + (cam.far - cam.near) * farLimits[0];
	bounds.y = cam.near + (cam.far - cam.near) * farLimits[1];
	bounds.z = cam.near + (cam.far - cam.near) * farLimits[2];
	bounds.w = cam.near + (cam.far - cam.near) * farLimits[3];
	return bounds;
}
