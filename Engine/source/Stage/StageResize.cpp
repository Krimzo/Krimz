#include "Stage/Stage.h"
#include "Window/Window.h"
#include "Render/Render.h"
#include "Editor/Picking.h"


void Krimz::Stage::Resize(const kl::uint2& siz) {
	if (siz.x > 0 && siz.y > 0) {
		Krimz::gpu->regenInternal(siz);
	}
};
