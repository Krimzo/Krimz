#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Shaders {
		namespace Vertex {
			inline ID3D11VertexShader* editor = nullptr;
			inline ID3D11VertexShader* shadow = nullptr;
			inline ID3D11VertexShader* index = nullptr;
			inline ID3D11VertexShader* outline = nullptr;
			inline ID3D11VertexShader* gizmo = nullptr;
		}
		namespace Pixel {
			inline ID3D11PixelShader* editor = nullptr;
			inline ID3D11PixelShader* shadow = nullptr;
			inline ID3D11PixelShader* index = nullptr;
			inline ID3D11PixelShader* outline = nullptr;
			inline ID3D11PixelShader* gizmo = nullptr;
		}
	}
}
