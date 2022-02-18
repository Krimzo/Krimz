#include "Engine/Engine.h"


void Start() {
	// Setting the icon
	win.setIcon("res/icons/k.ico");

	// Maximizing the window
	win.maximize();

	// Creating the gpu
	gpu = new kl::gpu(win.getHWND(), true);

	// On resize callback
	win.onResize = [&](const kl::ivec2& size) {
		gpu->regenBuffers(size);
		gpu->setViewport(kl::ivec2(0, 0), size);
		camera.aspect = float(size.x) / size.y;
	};

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
	camera.position = kl::vec3(-1.4f, 1.25f, 6.0f);
	camera.forward = kl::vec3(0.55f, -0.3f, -0.9f);

	/* DEBUG */
	kl::skybox* clouds = skyboxes.newInst(new kl::skybox(gpu->getDev(), gpu->getCon(), "Clouds",
		"res/textures/skyboxes/clouds/front.jpg",
		"res/textures/skyboxes/clouds/back.jpg",
		"res/textures/skyboxes/clouds/left.jpg",
		"res/textures/skyboxes/clouds/right.jpg",
		"res/textures/skyboxes/clouds/top.jpg",
		"res/textures/skyboxes/clouds/bottom.jpg"
	));
	kl::skybox* night = skyboxes.newInst(new kl::skybox(gpu->getDev(), gpu->getCon(), "Night",
		"res/textures/skyboxes/night/night.jpg"
	));
	skybox = clouds;
	kl::mesh* cube = gpu->newMesh("res/objects/cube.obj", true);
	kl::texture* peace = gpu->newTexture("res/textures/peace.jpg");
	kl::entity* mcube1 = entities.newInst(new kl::entity("Cube1", cube, peace));
	kl::entity* mcube2 = entities.newInst(new kl::entity("Cube2", cube, peace));
	mcube1->position.z = 2;
	mcube2->position.z = 2;
	mcube2->position.x = 2;
}
