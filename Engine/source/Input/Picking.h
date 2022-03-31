#pragma once

#include "Data/Entities.h"


namespace Engine {
	namespace Picking {
		inline int heldIndex = -1;
		inline int mouseIndex = -1;
		inline int selectedInd = -1;
		inline Engine::Entity* selected = nullptr;
		inline ID3D11Texture2D* texture = nullptr;
		inline ID3D11Texture2D* textureStag = nullptr;
		inline ID3D11RenderTargetView* targetV = nullptr;
	}
}
