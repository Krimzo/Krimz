#pragma once

#include "Types/Entity.h"


namespace Engine {
	namespace Picking {
		inline int heldIndex = -1;
		inline int mouseIndex = -1;
		inline int selectedInd = -1;
		inline ID3D11Texture2D* texture = nullptr;
		inline ID3D11Texture2D* textureStag = nullptr;
		inline ID3D11RenderTargetView* targetV = nullptr;

		void ReadObjectIndex();
	}
}
