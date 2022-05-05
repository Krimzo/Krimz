#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Data/Entities.h"


struct EDITOR_VS_CB {
	kl::mat4 w;
	kl::mat4 vpCam;
	kl::mat4 vpSun[4];
};
struct EDITOR_PS_CB {
	kl::float4 ambCol;
	kl::float4 dirCol;
	kl::float4 dirDir;
	kl::float4 camPos;
	kl::float4 rghFac;
	kl::float4 objInd;
	kl::float4 frFars;
	kl::mat4 camViewM;
};

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

	// Binding the shadow maps
	for (int i = 0; i < 4; i++) {
		Engine::Render::gpu->bindPixlTex(Engine::Light::sun->getShaderV(i), i + 1);
	}

	// Setting the camera data
	EDITOR_VS_CB editor_vert_data = {};
	editor_vert_data.vpCam = Engine::Selected::camera->matrix();
	for (int i = 0; i < 4; i++) {
		editor_vert_data.vpSun[i] = Engine::Light::sun->matrix(*Engine::Selected::camera, i);
	}

	// Setting the lighting data
	EDITOR_PS_CB editor_pixl_data = {};
	editor_pixl_data.ambCol = Engine::Light::ambient;
	editor_pixl_data.dirCol = Engine::Light::sun->color;
	editor_pixl_data.dirDir = Engine::Light::sun->getDir();
	editor_pixl_data.camPos = Engine::Selected::camera->position;
	editor_pixl_data.frFars = Engine::Light::sun->getFBounds(*Engine::Selected::camera);
	editor_pixl_data.camViewM = kl::mat4::lookAt(Engine::Selected::camera->position, Engine::Selected::camera->position + Engine::Selected::camera->getForward(), kl::float3::pos_y);;

	// Rendering entities
	for (int i = 0; auto & ent : Engine::entities) {
		if (ent->visible) {
			// Updating the vert data
			editor_vert_data.w = ent->matrix();
			Engine::Render::gpu->autoSetVertBuff(editor_vert_data);

			// Updating the pixl data
			editor_pixl_data.rghFac.x = ent->roughness;
			editor_pixl_data.objInd.x = float(i);
			Engine::Render::gpu->autoSetPixlBuff(editor_pixl_data);

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
