#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace DepthStencil {
		inline ID3D11DepthStencilState* disabled = nullptr;
		inline ID3D11DepthStencilState* depth = nullptr;
		inline ID3D11DepthStencilState* write = nullptr;
		inline ID3D11DepthStencilState* mask = nullptr;
	}
}
