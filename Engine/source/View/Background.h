#pragma once

#include "KrimzLib.h"


namespace Engine
{
	namespace Background
	{
		inline kl::color color = kl::color(20, 20, 20);
		inline kl::skybox* skybox = nullptr;
		inline kl::pbuffer<kl::skybox> skyboxes;
	}
}
