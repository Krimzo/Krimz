#include "Render/Render.h"
#include "Types/Camera.h"
#include "Types/Entity.h"


void Krimz::Render::Shadows()
{
	Krimz::gpu->viewport(kl::int2(0), Krimz::Light::sun->size());
	Krimz::gpu->bind(Krimz::Rasters::solid);
	Krimz::gpu->bind(Krimz::DepthStencil::depth);
	Krimz::gpu->bind(Krimz::Shaders::shadow);

	for (int i = 0; i < 4; i++)
	{
		Krimz::gpu->bindTargets({}, Krimz::Light::sun->depthView(i));
		Krimz::gpu->clear(Krimz::Light::sun->depthView(i));

		const kl::mat4 vpSun = Krimz::Light::sun->matrix(*Krimz::Selected::camera, i);
		for (auto& ent : Krimz::entities)
		{
			if (ent->shadows)
			{
				const kl::mat4 wvp = vpSun * ent->matrix();
				Krimz::gpu->autoVertexCBuffer(wvp);
				ent->render(false);
			}
		}
	}
}
