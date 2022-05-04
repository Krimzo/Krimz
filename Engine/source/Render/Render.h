#pragma once

#include "KrimzLib.h"
#include "Data/Cameras.h"
#include "Data/Light/Ambient.h"
#include "Data/Light/Direct.h"


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
		inline ID3D11RasterizerState* shadow = nullptr;
	}
	namespace DepthStencil {
		inline ID3D11DepthStencilState* disabled = nullptr;
		inline ID3D11DepthStencilState* depth = nullptr;
		inline ID3D11DepthStencilState* write = nullptr;
		inline ID3D11DepthStencilState* mask = nullptr;
	}
	namespace Shaders {
		namespace Vertex {
			inline ID3D11VertexShader* editor = nullptr;
			inline ID3D11VertexShader* shadow = nullptr;
			inline ID3D11VertexShader* index = nullptr;
			inline ID3D11VertexShader* outline = nullptr;
			inline ID3D11VertexShader* collider = nullptr;
			inline ID3D11VertexShader* gizmo = nullptr;
		}
		namespace Pixel {
			inline ID3D11PixelShader* editor = nullptr;
			inline ID3D11PixelShader* shadow = nullptr;
			inline ID3D11PixelShader* index = nullptr;
			inline ID3D11PixelShader* outline = nullptr;
			inline ID3D11PixelShader* collider = nullptr;
			inline ID3D11PixelShader* gizmo = nullptr;
		}
	}
	namespace CBuffers {
		inline ID3D11Buffer* buff16_1 = nullptr;
		inline ID3D11Buffer* buff16_2 = nullptr;
		inline ID3D11Buffer* buff32_1 = nullptr;
		inline ID3D11Buffer* buff32_2 = nullptr;
		inline ID3D11Buffer* buff48_1 = nullptr;
		inline ID3D11Buffer* buff48_2 = nullptr;
		inline ID3D11Buffer* buff64_1 = nullptr;
		inline ID3D11Buffer* buff64_2 = nullptr;
		inline ID3D11Buffer* buff96_1 = nullptr;
		inline ID3D11Buffer* buff96_2 = nullptr;
		inline ID3D11Buffer* buff176_1 = nullptr;
		inline ID3D11Buffer* buff176_2 = nullptr;
		inline ID3D11Buffer* buff384_1 = nullptr;
		inline ID3D11Buffer* buff384_2 = nullptr;
	}
	namespace Struct {
		struct DRAW_VS_CB {
			kl::mat4 w;
			kl::mat4 vpCam;
			kl::mat4 vpSun[4];
		};
		struct DRAW_PS_CB {
			kl::float4 ambCol;
			kl::float4 dirCol;
			kl::float4 dirDir;
			kl::float4 camPos;
			kl::float4 rghFac;
			kl::float4 objInd;
			kl::float4 frFars;
			kl::mat4 camViewM;
		};
		struct GIZM_PS_CB {
			kl::float4 objCol;
			kl::float4 objInd;
		};
	}
}
