#include "Engine/Engine.h"


void Start() {
	// Creating the gpu
	gpu = new kl::gpu(win.getHWND(), 4);

	// Creating the rasters
	solid_ra = gpu->newRaster(false, true);
	wire_ra = gpu->newRaster(true, true);
	solid_ra->bind();

	// Compiling shaders
	default_sh = gpu->newShaders("res/shaders/renderer.hlsl", sizeof(DEF_VS_CB), sizeof(DEF_PS_CB));
	highlight_sh = gpu->newShaders("res/shaders/highlight.hlsl", sizeof(kl::mat4), sizeof(kl::vec4));

	// Sampler setup
	kl::sampler* samp = gpu->newSampler(true, true);
	samp->bind(0);
}
