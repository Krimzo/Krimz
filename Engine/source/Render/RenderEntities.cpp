#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Data/Entities.h"


void Engine::Render::Entities() {
	// Binding render + index texture
	Engine::Render::gpu->bindTargets({ Engine::Render::targetV, Engine::Picking::targetV });

	// Clearing picking tex
	Engine::Render::gpu->clear(Engine::Picking::targetV, kl::float4(-1.0f));

	// Raster bind
	Engine::Render::gpu->bind(Engine::Render::entityRaster);

	// Depth bind
	Engine::Render::gpu->bind(Engine::DepthStencil::depth);

	// Binding the editor shaders
	Engine::Render::gpu->bind(Engine::Shaders::Vertex::editor);
	Engine::Render::gpu->bind(Engine::Shaders::Pixel::editor);
	Engine::Render::gpu->bindVertCBuff(Engine::CBuffers::buff384_1, 0);
	Engine::Render::gpu->bindPixlCBuff(Engine::CBuffers::buff176_1, 0);

	// Binding the shadow maps
	for (int i = 0; i < 4; i++) {
		Engine::Render::gpu->bindPixlTex(Engine::Light::sun->getShaderV(i), i + 1);
	}

	// Setting the camera data
	Engine::Struct::DRAW_VS_CB draw_vert_data = {};
	draw_vert_data.vpCam = Engine::Selected::camera->matrix();
	for (int i = 0; i < 4; i++) {
		draw_vert_data.vpSun[i] = Engine::Light::sun->matrix(*Engine::Selected::camera, i);
	}

	// Setting the lighting data
	Engine::Struct::DRAW_PS_CB draw_pixl_data = {};
	draw_pixl_data.ambCol = Engine::Light::ambient;
	draw_pixl_data.dirCol = Engine::Light::sun->color;
	draw_pixl_data.dirDir = Engine::Light::sun->getDir();
	draw_pixl_data.camPos = Engine::Selected::camera->position;
	draw_pixl_data.frFars = Engine::Light::sun->getFBounds(*Engine::Selected::camera);
	draw_pixl_data.camViewM = kl::mat4::lookAt(Engine::Selected::camera->position, Engine::Selected::camera->position + Engine::Selected::camera->getForward(), kl::float3::pos_y);;

	// Rendering entities
	for (int i = 0; auto & ent : Engine::entities) {
		if (ent->visible) {
			// Updating the vert data
			draw_vert_data.w = ent->matrix();
			Engine::Render::gpu->setBuffData(Engine::CBuffers::buff384_1, &draw_vert_data);

			// Updating the pixl data
			draw_pixl_data.rghFac.x = ent->roughness;
			draw_pixl_data.objInd.x = float(i);
			Engine::Render::gpu->setBuffData(Engine::CBuffers::buff176_1, &draw_pixl_data);

			// Rendering the entity
			if (ent == Engine::Selected::entity) {
				Engine::Render::gpu->bind(Engine::DepthStencil::write);
				ent->render(true);
				Engine::Render::gpu->bind(Engine::DepthStencil::depth);
			}
			else {
				ent->render(true);
			}
		}

		// Index
		i++;
	}
}
