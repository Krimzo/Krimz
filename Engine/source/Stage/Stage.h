#pragma once

#include "KrimzLib.h"


namespace Engine {
	inline bool gameRunning = false;
	namespace Stage {
		void Start();
		void Update();
		void Resize(const kl::uint2& siz);
		void End();
	}
}
