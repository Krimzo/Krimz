#pragma once

#include "KrimzLib.h"


namespace Engine::Light {
	class Direct {
	private:
		kl::uint2 m_TextureSize;
		kl::dx::view::depth m_DepthViews[4] = {};
		kl::dx::view::shader m_ShaderViews[4] = {};
		kl::float3 m_Direction = { 0.0f, -1.0f, -2.0f };

	public:
		float farLimits[4] = { 0.05f, 0.15f, 0.45f, 1.0f };
		kl::float4 color = kl::colors::white;

		Direct(const kl::uint2& size);
		Direct(const Direct&) = delete;
		void operator=(const Direct&) = delete;
		~Direct();

		kl::uint2 size() const;

		kl::float3 direction() const;
		void direction(const kl::float3& dir);

		kl::mat4 matrix(const kl::camera& cam, uint ind) const;

		kl::dx::view::depth depthView(uint ind) const;
		kl::dx::view::shader shaderView(uint ind) const;

		kl::float4 frustumBounds(const kl::camera& cam) const;
	};
}
