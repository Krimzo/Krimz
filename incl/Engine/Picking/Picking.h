#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Picking {
		inline int heldIndex = -1;
		inline int mouseIndex = -1;
		inline int selectedInd = -1;
		inline kl::entity* selected = nullptr;
		inline ID3D11Texture2D* texture = nullptr;
		inline ID3D11Texture2D* textureStag = nullptr;
		inline ID3D11RenderTargetView* targetV = nullptr;
	}
}
