#pragma once

#include "KrimzLib.h"


namespace Engine::Light {
	class Direct {
	private:
		kl::int2 texSize;
		ID3D11DepthStencilView* depthVs[4] = {};
		ID3D11ShaderResourceView* shaderVs[4] = {};
		kl::float3 direction = kl::float3(0.0f, -1.0f, -2.0f);

	public:
		float farLimits[4] = { 0.05f, 0.15f, 0.45f, 1.0f };
		kl::float4 color = kl::colors::white;

		Direct(const kl::int2& size);
		Direct(const Direct&) = delete;
		void operator=(const Direct&) = delete;
		~Direct();

		kl::int2 getSize() const;

		kl::float3 getDir() const;
		void setDir(const kl::float3& dir);

		kl::mat4 matrix(const kl::camera& cam, uint32_t ind) const;

		ID3D11DepthStencilView* getDepthV(uint32_t ind) const;
		ID3D11ShaderResourceView* getShaderV(uint32_t ind) const;

		kl::float4 getFBounds(const kl::camera& cam) const;
	};
}
