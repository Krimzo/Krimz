#include "Engine/Engine.h"


void Resize(const kl::int2& siz) {
	if (siz.x > 0 && siz.y > 0) {
		gpu->regenBuffers(siz);
		gpu->viewport(kl::int2(0, 0), siz);
		camera.aspect = float(siz.x) / siz.y;

		delete outlineBuff;
		outlineBuff = new kl::ibuffer(gpu->dev(), gpu->con(), siz.x, siz.y);
	}
};
