#include "Engine/Engine.h"


void Engine::Update::Draw() {
	// Binding internal + index texture
	Engine::Render::gpu->bindInternal({ Engine::Picking::targetV });

	// Clearing picking tex
	Engine::Render::gpu->clear(Engine::Picking::targetV, kl::float4(-1.0f));

	// Binding the editor shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::editor);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::editor);
	Engine::Render::gpu->bindVertCBuff(Engine::CBuffers::Vertex::editor, 0);
	Engine::Render::gpu->bindPixlCBuff(Engine::CBuffers::Pixel::editor, 0);

	// Binding the shadow map
	Engine::Render::gpu->bindPixlTex(Engine::Light::sun.shadowMapSV, 1);

	// Setting the camera data
	Engine::Struct::DRAW_VS_CB draw_vert_data = {};
	draw_vert_data.vpCam = Engine::Render::camera.matrix();
	draw_vert_data.vpSun = Engine::Light::sun.matrix(Engine::Render::camera);

	// Setting the lighting data
	Engine::Struct::DRAW_PS_CB draw_pixl_data = {};
	draw_pixl_data.ambCol = Engine::Light::ambient.getCol();
	draw_pixl_data.dirCol = Engine::Light::sun.getCol();
	draw_pixl_data.dirDir = Engine::Light::sun.getDir();
	draw_pixl_data.camPos = Engine::Render::camera.position;

	// Rendering entities
	for (int i = 0; i < Engine::Game::entities.size(); i++) {
		if (Engine::Game::entities[i]->visible) {
			// Updating the vert data
			draw_vert_data.w = Engine::Game::entities[i]->matrix();
			Engine::Render::gpu->setBuffData(Engine::CBuffers::Vertex::editor, &draw_vert_data);

			// Updating the pixl data
			draw_pixl_data.rghFac.x = Engine::Game::entities[i]->roughness;
			draw_pixl_data.objInd.x = float(i);
			Engine::Render::gpu->setBuffData(Engine::CBuffers::Pixel::editor, &draw_pixl_data);

			// Rendering the entity
			if (Engine::Game::entities[i] == Engine::Picking::selected) {
				Engine::Render::gpu->bind(Engine::DepthStencil::write);
				Engine::Game::entities[i]->render(Engine::Render::gpu, true);
				Engine::Render::gpu->bind(Engine::DepthStencil::depth);
			}
			else {
				Engine::Game::entities[i]->render(Engine::Render::gpu, true);
			}
		}
	}
}
