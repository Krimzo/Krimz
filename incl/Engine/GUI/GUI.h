#pragma once

#include "ImGui/imgui.h"
#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace GUI {
		// Icon textures
		inline ID3D11ShaderResourceView* folderIcon = nullptr;
		inline ID3D11ShaderResourceView* folderEIcon = nullptr;
		inline ID3D11ShaderResourceView* fileIcon = nullptr;
		inline ID3D11ShaderResourceView* imageIcon = nullptr;
		inline ID3D11ShaderResourceView* objectIcon = nullptr;
		inline ID3D11ShaderResourceView* codeIcon = nullptr;

		// Viewport
		inline bool viewportFocus = false;
		inline kl::int2 viewportPos;
		inline kl::int2 viewportSize;
	}
}
