#include "Engine/Engine.h"


void Start() {
	// Setting the icon
	win.setIcon("res/icons/k.ico");

	// Maximizing the window
	win.maximize();

	// Creating the gpu
	gpu = new kl::gpu(win.getWND(), true);

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
	editor_sh = gpu->newShaders("res/shaders/editor.hlsl", sizeof(DRAW_VS_CB), sizeof(DRAW_PS_CB));
	highlight_sh = gpu->newShaders("res/shaders/highlight.hlsl", sizeof(HIGH_VS_CB), sizeof(HIGH_PS_CB));
	gizmo_sh = gpu->newShaders("res/shaders/gizmo.hlsl", sizeof(GIZM_VS_CB), sizeof(GIZM_PS_CB));

	// Sampler setup
	kl::sampler* samp = gpu->newSampler(true, true);
	samp->bind(0);

	// Camera setup
	camera.position = kl::vec3(-1.4f, 1.25f, 6.0f);
	camera.forward = kl::vec3(0.55f, -0.3f, -0.9f);

	// Gizmo meshe loading
	gizmo_scale = gpu->newMesh("res/objects/gizmos/scale.obj", true);
	gizmo_move = gpu->newMesh("res/objects/gizmos/move.obj", true);
	gizmo_rotate = gpu->newMesh("res/objects/gizmos/rotate.obj", true);

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


	kl::mesh* monke = gpu->newMesh("res/objects/monke.obj", true);
	kl::texture* check = gpu->newTexture("res/textures/checkers.jpg");

	const int size = 3;

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			const int i = y * size + x;
			kl::entity* temp = entities.newInst(new kl::entity("Monke" + std::to_string(i), monke, check));
			temp->position = kl::vec3(x * 2.0f, y * 2.0f, 0);
			temp->rotation.y = 180;
		}
	}
}
