#include "Input/Picking.h"
#include "Utility/Window.h"
#include "Render/Render.h"


void Engine::Picking::ReadObjectIndex()
{
	if (Engine::win.mouse.position.x >= 0 && Engine::win.mouse.position.x < Engine::win.getSize().x &&
		Engine::win.mouse.position.y >= 0 && Engine::win.mouse.position.y < Engine::win.getSize().y)
	{
		// Copying the index texture pixel
		D3D11_BOX srcBox = {};
		srcBox.left = Engine::win.mouse.position.x;
		srcBox.right = srcBox.left + 1;
		srcBox.top = Engine::win.mouse.position.y;
		srcBox.bottom = srcBox.top + 1;
		srcBox.front = 0;
		srcBox.back = 1;
		Engine::Render::gpu->con()->CopySubresourceRegion(
			Engine::Picking::textureStag, 0, 0, 0, 0, Engine::Picking::texture, 0, &srcBox);

		// Mapping the staging texture
		D3D11_MAPPED_SUBRESOURCE msr = {};
		Engine::Render::gpu->con()->Map(Engine::Picking::textureStag, 0, D3D11_MAP_READ, NULL, &msr);

		// Reading the bytes
		float index = 0;
		memcpy(&index, msr.pData, sizeof(float));

		// Unmapping the staging texture
		Engine::Render::gpu->con()->Unmap(Engine::Picking::textureStag, NULL);

		// Saving mouse object index
		Engine::Picking::mouseIndex = int(index);
	}
	else
	{
		Engine::Picking::mouseIndex = -1;
	}
}
