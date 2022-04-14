#pragma once

#include "KrimzLib.h"


namespace Engine
{
	namespace Render
	{
		inline kl::gpu* gpu = nullptr;
		inline kl::camera camera;

		inline ID3D11RasterizerState* entityRaster = nullptr;

		void Shadows();
		void Entity();
		void Outline();
		void Gizmo();
		void GUI();
	}
	namespace Rasters
	{
		inline ID3D11RasterizerState* solid = nullptr;
		inline ID3D11RasterizerState* wire = nullptr;
		inline ID3D11RasterizerState* shadow = nullptr;
	}
	namespace DepthStencil
	{
		inline ID3D11DepthStencilState* disabled = nullptr;
		inline ID3D11DepthStencilState* depth = nullptr;
		inline ID3D11DepthStencilState* write = nullptr;
		inline ID3D11DepthStencilState* mask = nullptr;
	}
	namespace Shaders
	{
		namespace Vertex
		{
			inline ID3D11VertexShader* editor = nullptr;
			inline ID3D11VertexShader* shadow = nullptr;
			inline ID3D11VertexShader* index = nullptr;
			inline ID3D11VertexShader* outline = nullptr;
			inline ID3D11VertexShader* gizmo = nullptr;
		}
		namespace Pixel
		{
			inline ID3D11PixelShader* editor = nullptr;
			inline ID3D11PixelShader* shadow = nullptr;
			inline ID3D11PixelShader* index = nullptr;
			inline ID3D11PixelShader* outline = nullptr;
			inline ID3D11PixelShader* gizmo = nullptr;
		}
	}
	namespace CBuffers
	{
		namespace Vertex
		{
			inline ID3D11Buffer* editor = nullptr;
			inline ID3D11Buffer* shadow = nullptr;
			inline ID3D11Buffer* index = nullptr;
			inline ID3D11Buffer* gizmo = nullptr;
		}
		namespace Pixel
		{
			inline ID3D11Buffer* editor = nullptr;
			inline ID3D11Buffer* outline = nullptr;
			inline ID3D11Buffer* gizmo = nullptr;
		}
	}
}
