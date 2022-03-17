#pragma once

#include "KrimzLib/KrimzLib.hpp"

#include "Engine/Data/Struct.h"
#include "Engine/Render/Rasters.h"
#include "Engine/Render/DStates.h"
#include "Engine/Render/Shaders.h"
#include "Engine/Render/CBuffers.h"
#include "Engine/Callbacks/Callbacks.h"
#include "Engine/Picking/Picking.h"
#include "Engine/Picking/Outline.h"
#include "Engine/Edit/Gizmo.h"


namespace Engine {
	namespace Window {
		inline kl::window win;
		inline kl::int2 size;
	}
	namespace Input {
		void Setup();
	}
	namespace Render {
		inline kl::gpu* gpu = nullptr;
		inline kl::camera camera;
	}
	namespace Time {
		inline kl::timer timer;
		inline float delta = 0;
		inline float elapsed = 0;
	}
	namespace Background {
		inline kl::color color = kl::colors::gray;
		inline kl::skybox* skybox = nullptr;
		inline kl::pbuffer<kl::skybox> skyboxes;
	}
	namespace Game {
		inline kl::pbuffer<kl::entity> entities;
		inline bool running = false;
	}
	namespace Light {
		inline kl::ambient ambient;
		inline kl::direct sun;
	}
}
