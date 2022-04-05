#pragma once

#include "KrimzLib.h"


namespace Engine
{
	namespace Stage
	{
		void Start();
		void Update();
		void Resize(const kl::int2& siz);
		void End();
	}
	inline bool gameRunning = false;
}
