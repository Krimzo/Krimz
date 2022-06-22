#pragma once

#include "Types/Entity.h"


namespace Engine {
	namespace Picking {
		inline int heldIndex = -1;
		inline int mouseIndex = -1;
		inline int selectedIndex = -1;
		inline kl::dx::texture texture = nullptr;
		inline kl::dx::texture textureStaging = nullptr;
		inline kl::dx::view::target targetView = nullptr;

		void ReadObjectIndex();
	}
}
