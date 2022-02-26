#include "Engine/Engine.h"


void Start() {
	// Setting the icon
	win.setIcon("res/icons/k.ico");

	// Maximizing the window
	win.maximize();

	// On resize callback
	win.onResize = [&](const kl::ivec2& size) {
		if (size.x > 0 && size.y > 0) {
			gpu->regenBuffers(size);
			gpu->setViewport(kl::ivec2(0, 0), size);
			camera.aspect = float(size.x) / size.y;
		}
	};

	// Creating the gpu
	gpu = new kl::gpu(win.getWND(), true);

	// Creating the rasters
	solid_ra = gpu->newRaster(false, true);
	wire_ra = gpu->newRaster(true, true);
	shadow_ra = gpu->newRaster(false, true, false);
	solid_ra->bind();

	// Compiling shaders
	editor_sh = gpu->newShaders("res/shaders/editor.hlsl", sizeof(DRAW_VS_CB), sizeof(DRAW_PS_CB));
	shadow_sh = gpu->newShaders("res/shaders/shadows.hlsl", sizeof(kl::mat4), 0);
	outline_sh = gpu->newShaders("res/shaders/outline.hlsl", sizeof(kl::mat4), sizeof(kl::vec4));
	gizmo_sh = gpu->newShaders("res/shaders/gizmo.hlsl", sizeof(kl::mat4), sizeof(GIZM_PS_CB));

	// Sampler setup
	kl::sampler* samp = gpu->newSampler(true, true);
	samp->bind(0);

	// Camera setup
	camera.position = kl::vec3(-1.4f, 1.25f, -6.0f);
	camera.forward = kl::vec3(0.55f, -0.3f, 0.9f);

	// Sun setup
	sun.shadowMap = gpu->newSBuffer(4096);
	sun.direction = kl::vec3(0.575f, -0.75f, 2.0f);

	// Gizmo mesh loading
	gizmo_scale = gpu->newMesh("res/objects/gizmos/scale.obj", true);
	gizmo_move = gpu->newMesh("res/objects/gizmos/move.obj", true);
	gizmo_rotate = gpu->newMesh("res/objects/gizmos/rotate.obj", true);

	/* DEBUG */
	// Skybox
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

	// Mesh
	kl::mesh* cube_mes = gpu->newMesh("res/objects/cube.obj");
	kl::mesh* monke_mes = gpu->newMesh("res/objects/monke.obj");
	kl::mesh* horse_mes = gpu->newMesh("res/objects/horse.obj");

	// Texture
	kl::texture* lgray_tex = gpu->newTexture(kl::image(kl::ivec2(1, 1), kl::colors::lgray));
	kl::texture* checker_tex = gpu->newTexture("res/textures/checkers.jpg");
	kl::texture* horse_tex = gpu->newTexture("res/textures/horse.jpg");

	// Entity
	kl::entity* plane = entities.newInst(new kl::entity("Plane", cube_mes, lgray_tex));
	plane->size = kl::vec3(50.0f, 0.25f, 50.0f);
	plane->position.y = -2.0f;
	plane->roughness = 0.9f;

	kl::entity* horse1 = entities.newInst(new kl::entity("Horse1", horse_mes, horse_tex));
	kl::entity* horse2 = entities.newInst(new kl::entity("Horse2", horse_mes, horse_tex));
	horse1->size *= 50.0f;
	horse2->size *= 50.0f;
	horse1->position.y = -1.8f;
	horse2->position.y = -1.8f;
	horse1->position.x =  15.0f;
	horse2->position.x = -15.0f;
	horse2->rotation.y = 180.0f;

	const int size = 3;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			const int i = y * size + x;
			kl::entity* temp = entities.newInst(new kl::entity("Monke" + std::to_string(i), monke_mes, checker_tex));
			temp->position = kl::vec3((x - 1.0f) * 2.0f, y * 2.0f, y * 2.0f);
		}
	}
}
