#pragma once

#include "KrimzLib.h"
#include "Types/Light/Ambient.h"
#include "Types/Light/Direct.h"


namespace Engine {
	inline kl::window win;
	namespace Time {
		inline kl::timer timer;
		inline float delta = 0;
		inline float elapsed = 0;
	}
	namespace Render {
		inline bool vSync = true;
		inline std::unique_ptr<kl::gpu> gpu = nullptr;
		inline ID3D11RasterizerState* entityRaster = nullptr;

		inline kl::int2 targetSize = kl::int2(100);
		inline ID3D11RenderTargetView* targetV = nullptr;
		inline ID3D11ShaderResourceView* shaderV = nullptr;

		void Shadows();
		void FixViewport();
		void Skybox();
		void Entities();
		void Outline();
		void Collider();
		void Gizmo();
		void GUI();
	}
	namespace Light {
		inline Ambient ambient = kl::float4(0.1f);
		inline std::shared_ptr<Direct> sun;
	}
	namespace Outline {
		inline ID3D11Buffer* screenM = nullptr;
		inline ID3D11RenderTargetView* targetV = nullptr;
		inline ID3D11ShaderResourceView* shaderV = nullptr;
		inline kl::color color = kl::colors::orange;
		inline kl::color colliderColor = kl::colors::cyan;
		inline bool renderColliders = true;
	}
	namespace Rasters {
		inline ID3D11RasterizerState* solid = nullptr;
		inline ID3D11RasterizerState* wire = nullptr;
		inline ID3D11RasterizerState* skybox = nullptr;
	}
	namespace DepthStencil {
		inline ID3D11DepthStencilState* disabled = nullptr;
		inline ID3D11DepthStencilState* depth = nullptr;
		inline ID3D11DepthStencilState* write = nullptr;
		inline ID3D11DepthStencilState* mask = nullptr;
	}
	namespace Shaders {
		inline kl::shaders editor = {};
		inline kl::shaders shadow = {};
		inline kl::shaders index = {};
		inline kl::shaders outline = {};
		inline kl::shaders collider = {};
		inline kl::shaders gizmo = {};
	}
}
