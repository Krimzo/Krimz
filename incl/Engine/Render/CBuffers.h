#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace CBuffers {
		namespace Vertex {
			inline ID3D11Buffer* editor = nullptr;
			inline ID3D11Buffer* shadow = nullptr;
			inline ID3D11Buffer* index = nullptr;
			inline ID3D11Buffer* gizmo = nullptr;
		}
		namespace Pixel {
			inline ID3D11Buffer* editor = nullptr;
			inline ID3D11Buffer* outline = nullptr;
			inline ID3D11Buffer* gizmo = nullptr;
		}
	}
}
