#include "Stage/Stage.h"
#include "Window/Window.h"
#include "Render/Render.h"
#include "Editor/Picking.h"


void Engine::Stage::Resize(const kl::int2& siz) {
	if (siz.x > 0 && siz.y > 0) {
		Engine::Render::gpu->regenBuffers(siz);
	}
};
