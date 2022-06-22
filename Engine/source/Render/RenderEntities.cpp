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
	Engine::gpu->bindTargets({ Engine::Render::targetView, Engine::Picking::targetView });
	Engine::gpu->clear(Engine::Picking::targetView, -1.0f);
	Engine::gpu->bind(Engine::Selected::raster);
	Engine::gpu->bind(Engine::DepthStencil::depth);
	Engine::gpu->bind(Engine::Shaders::editor);

	for (int i = 0; i < 4; i++) {
		Engine::gpu->bindPixelShaderView(Engine::Light::sun->shaderView(i), i + 3);
	}

	EDITOR_VS_CB editor_vert_data = {};
	editor_vert_data.vpCameraMatrix = Engine::Selected::camera->matrix();
	for (int i = 0; i < 4; i++) {
		editor_vert_data.vpSunMatrices[i] = Engine::Light::sun->matrix(*Engine::Selected::camera, i);
	}

	EDITOR_PS_CB editor_pixl_data = {};
	editor_pixl_data.ambientColor = Engine::Light::ambient;
	editor_pixl_data.sunColor = Engine::Light::sun->color;
	editor_pixl_data.sunDirection = kl::float4(Engine::Light::sun->direction(), 0.0f);
	editor_pixl_data.cameraPosition = kl::float4(Engine::Selected::camera->position, 0.0f);
	editor_pixl_data.frustumFarPlanes = Engine::Light::sun->frustumBounds(*Engine::Selected::camera);
	editor_pixl_data.cameraViewMatrix = kl::mat4::lookAt(Engine::Selected::camera->position, Engine::Selected::camera->position + Engine::Selected::camera->forward(), { 0.0f, 1.0f, 0.0f });

	for (int i = 0; auto & ent : Engine::entities) {
		if (ent->visible) {
			editor_vert_data.wMatrix = ent->matrix();
			Engine::gpu->autoVertexCBuffer(editor_vert_data);

			editor_pixl_data.multiData.x = float(i);
			editor_pixl_data.multiData.y = float(ent->material.hasNormalMap());
			editor_pixl_data.multiData.z = float(ent->material.hasRoughnessMap());
			editor_pixl_data.multiData.w = ent->material.roughness;
			Engine::gpu->autoPixelCBuffer(editor_pixl_data);

			if (ent == Engine::Selected::entity) {
				Engine::gpu->bind(Engine::DepthStencil::write);
				ent->render(true);
				Engine::gpu->bind(Engine::DepthStencil::depth);
			}
			else {
				ent->render(true);
			}
		}

		i++;
	}
}
