#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"
#include "Types/Entity.h"


struct EDITOR_VS_CB {
	kl::mat4 wMatrix;
	kl::mat4 vpCameraMatrix;
	kl::mat4 vpSunMatrices[4];
};
struct EDITOR_PS_CB {
	kl::float4 multiData;
	kl::float4 ambientColor;
	kl::float4 sunColor;
	kl::float4 sunDirection;
	kl::float4 cameraPosition;
	kl::float4 frustumFarPlanes;
	kl::mat4 cameraViewMatrix;
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
	Engine::Render::gpu->bind(Engine::Shaders::editor);

	// Binding the shadow maps
	for (int i = 0; i < 4; i++) {
		Engine::Render::gpu->bindPixlTex(Engine::Light::sun->getShaderV(i), i + 3);
	}

	// Setting the camera data
	EDITOR_VS_CB editor_vert_data = {};
	editor_vert_data.vpCameraMatrix = Engine::Selected::camera->matrix();
	for (int i = 0; i < 4; i++) {
		editor_vert_data.vpSunMatrices[i] = Engine::Light::sun->matrix(*Engine::Selected::camera, i);
	}

	// Setting the lighting data
	EDITOR_PS_CB editor_pixl_data = {};
	editor_pixl_data.ambientColor = Engine::Light::ambient;
	editor_pixl_data.sunColor = Engine::Light::sun->color;
	editor_pixl_data.sunDirection = Engine::Light::sun->getDir();
	editor_pixl_data.cameraPosition = Engine::Selected::camera->position;
	editor_pixl_data.frustumFarPlanes = Engine::Light::sun->getFBounds(*Engine::Selected::camera);
	editor_pixl_data.cameraViewMatrix = kl::mat4::lookAt(Engine::Selected::camera->position, Engine::Selected::camera->position + Engine::Selected::camera->getForward(), kl::float3::pos_y);

	// Rendering entities
	for (int i = 0; auto & ent : Engine::entities) {
		if (ent->visible) {
			// Updating the vert data
			editor_vert_data.wMatrix = ent->matrix();
			Engine::Render::gpu->autoVertexCBuffer(editor_vert_data);

			// Updating the pixl data
			editor_pixl_data.multiData.x = float(i);
			editor_pixl_data.multiData.y = float(ent->material.hasNormalMap());
			editor_pixl_data.multiData.z = float(ent->material.hasRoughnessMap());
			editor_pixl_data.multiData.w = ent->material.roughness;
			Engine::Render::gpu->autoPixelCBuffer(editor_pixl_data);

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
