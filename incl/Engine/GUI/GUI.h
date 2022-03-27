#pragma once

#include "ImGui/imgui.h"
#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace GUI {
		// Icon textures
		inline ID3D11ShaderResourceView* folderTex = nullptr;
		inline ID3D11ShaderResourceView* emptyFolderTex = nullptr;
		inline ID3D11ShaderResourceView* fileTex = nullptr;

		// Viewport
		inline bool viewportFocus = false;
		inline kl::int2 viewportPos;
		inline kl::int2 viewportSize;
	}
}
