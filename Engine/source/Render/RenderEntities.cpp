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

void Krimz::Render::Entities() {
	Krimz::gpu->bindTargets({ Krimz::Render::targetView, Krimz::Picking::targetView });
	Krimz::gpu->clear(Krimz::Picking::targetView, -1.0f);
	Krimz::gpu->bind(Krimz::Selected::raster);
	Krimz::gpu->bind(Krimz::DepthStencil::depth);
	Krimz::gpu->bind(Krimz::Shaders::editor);

	for (int i = 0; i < 4; i++) {
		Krimz::gpu->bindPixelShaderView(Krimz::Light::sun->shaderView(i), i + 3);
	}

	EDITOR_VS_CB editor_vert_data = {};
	editor_vert_data.vpCameraMatrix = Krimz::Selected::camera->matrix();
	for (int i = 0; i < 4; i++) {
		editor_vert_data.vpSunMatrices[i] = Krimz::Light::sun->matrix(*Krimz::Selected::camera, i);
	}

	EDITOR_PS_CB editor_pixl_data = {};
	editor_pixl_data.ambientColor = Krimz::Light::ambient;
	editor_pixl_data.sunColor = Krimz::Light::sun->color;
	editor_pixl_data.sunDirection = kl::float4(Krimz::Light::sun->direction(), 0.0f);
	editor_pixl_data.cameraPosition = kl::float4(Krimz::Selected::camera->position, 0.0f);
	editor_pixl_data.frustumFarPlanes = Krimz::Light::sun->frustumBounds(*Krimz::Selected::camera);
	editor_pixl_data.cameraViewMatrix = kl::mat4::lookAt(Krimz::Selected::camera->position, Krimz::Selected::camera->position + Krimz::Selected::camera->forward(), { 0.0f, 1.0f, 0.0f });

	for (int i = 0; auto & ent : Krimz::entities) {
		if (ent->visible) {
			editor_vert_data.wMatrix = ent->matrix();
			Krimz::gpu->autoVertexCBuffer(editor_vert_data);

			editor_pixl_data.multiData.x = float(i);
			editor_pixl_data.multiData.y = float(ent->material.hasNormalMap());
			editor_pixl_data.multiData.z = float(ent->material.hasRoughnessMap());
			editor_pixl_data.multiData.w = ent->material.roughness;
			Krimz::gpu->autoPixelCBuffer(editor_pixl_data);

			if (ent == Krimz::Selected::entity) {
				Krimz::gpu->bind(Krimz::DepthStencil::write);
				ent->render(true);
				Krimz::gpu->bind(Krimz::DepthStencil::depth);
			}
			else {
				ent->render(true);
			}
		}

		i++;
	}
}
