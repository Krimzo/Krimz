#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Background {
		inline kl::color color = kl::color(30, 30, 30);
		inline kl::skybox* skybox = nullptr;
		inline kl::pbuffer<kl::skybox> skyboxes;
	}
}
