#pragma once

#include "Game/Entity.h"
#include "KrimzLib.h"


namespace Engine {
	namespace Game {
		inline bool running = false;
	}
	inline kl::pbuffer<Engine::Entity> entities;
}
