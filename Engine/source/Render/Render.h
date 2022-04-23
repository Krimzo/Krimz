#pragma once

#include "KrimzLib.h"


namespace Engine {
	namespace Render {
		inline kl::gpu* gpu = nullptr;
		inline kl::camera camera;

		inline ID3D11RasterizerState* entityRaster = nullptr;

		inline kl::color outlineCol = kl::colors::orange;
		inline kl::color collOutlineCol = kl::colors::cyan;
		inline bool colliders = false;

		inline ID3D11Buffer* screenM = nullptr;
		inline ID3D11RenderTargetView* targetV = nullptr;
		inline ID3D11ShaderResourceView* shaderV = nullptr;

		void Shadows();
		void Entity();
		void Outline();
		void Collider();
		void Gizmo();
		void GUI();
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
		inline ID3D11Buffer* buff192_1 = nullptr;
		inline ID3D11Buffer* buff192_2 = nullptr;
	}
}
