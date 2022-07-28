#pragma once

#include "KrimzLib.h"


namespace Krimz
{
	class Renderable
	{
	public:
		virtual void render(kl::gpu& gpu) = 0;
	};
}
