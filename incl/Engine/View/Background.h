#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Background {
		inline kl::color color = kl::colors::gray;
		inline kl::skybox* skybox = nullptr;
		inline kl::pbuffer<kl::skybox> skyboxes;
	}
}
