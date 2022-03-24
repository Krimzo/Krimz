#pragma once

#include "Engine/Game/Entity.h"
#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Game {
		inline kl::pbuffer<Engine::Game::Entity> entities;
		inline bool running = false;
	}
}
