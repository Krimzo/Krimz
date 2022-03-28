#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Background {
		inline kl::color color = kl::color(15, 15, 15);
		inline kl::skybox* skybox = nullptr;
		inline kl::pbuffer<kl::skybox> skyboxes;
	}
}
