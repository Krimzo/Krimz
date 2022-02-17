#include "Engine/Engine.h"


void Start() {
	// Creating the gpu
	gpu = new kl::gpu(win.getHWND(), true);

	// Creating the rasters
	solid_ra = gpu->newRaster(false, true);
	wire_ra = gpu->newRaster(true, true);
	solid_ra->bind();

	// Compiling shaders
	editor_sh = gpu->newShaders("res/shaders/editor.hlsl", sizeof(EDI_VS_CB), sizeof(EDI_PS_CB));
	highlight_sh = gpu->newShaders("res/shaders/highlight.hlsl", sizeof(kl::mat4), sizeof(kl::vec4));

	// Sampler setup
	kl::sampler* samp = gpu->newSampler(true, true);
	samp->bind(0);

	// Camera setup
	camera.position = kl::vec3(-1.4f, 1.25f, -1.1f);
	camera.forward = kl::vec3(0.35f, -0.3f, 0.9f);

	/* DEBUG */
	kl::mesh* cube = gpu->newMesh("res/objects/cube.obj", true);
	kl::texture* dogo = gpu->newTexture("res/textures/dogo.jpg");
	kl::entity* cogo1 = entities.newInst(new kl::entity(cube, dogo));
	kl::entity* cogo2 = entities.newInst(new kl::entity(cube, dogo));
	cogo1->position.z = 2;
	cogo2->position.z = 2;
	cogo2->position.x = 2;
}
