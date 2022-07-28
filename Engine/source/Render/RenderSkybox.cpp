#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Types/Camera.h"


void Krimz::Render::Skybox()
{
	Krimz::gpu->bindTargets({ Krimz::Render::targetView });
	Krimz::gpu->bind(Krimz::Rasters::skybox);
	Krimz::gpu->bind(Krimz::DepthStencil::disabled);
	Krimz::Selected::camera->skybox->render(Krimz::Selected::camera->matrix());
}
