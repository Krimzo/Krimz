#include "Editor/Picking.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void Engine::Picking::ReadObjectIndex() {
	const kl::int2 indexPos = Engine::win.mouse.position - Engine::GUI::viewportPos;
	if (indexPos.x >= 0 && indexPos.x < Engine::GUI::viewportSize.x && indexPos.y >= 0 && indexPos.y < Engine::GUI::viewportSize.y) {
		// Copying the index texture pixel
		D3D11_BOX srcBox = {};
		srcBox.left = indexPos.x;
		srcBox.right = srcBox.left + 1;
		srcBox.top = indexPos.y;
		srcBox.bottom = srcBox.top + 1;
		srcBox.front = 0;
		srcBox.back = 1;
		Engine::Render::gpu->con()->CopySubresourceRegion(Engine::Picking::textureStag, 0, 0, 0, 0, Engine::Picking::texture, 0, &srcBox);

		// Mapping the staging texture
		D3D11_MAPPED_SUBRESOURCE msr = {};
		Engine::Render::gpu->con()->Map(Engine::Picking::textureStag, 0, D3D11_MAP_READ, NULL, &msr);

		// Reading the index
		float index = 0;
		memcpy(&index, msr.pData, sizeof(float));

		// Unmapping the staging texture
		Engine::Render::gpu->con()->Unmap(Engine::Picking::textureStag, NULL);

		// Saving mouse object index
		Engine::Picking::mouseIndex = int(index);
	}
	else {
		Engine::Picking::mouseIndex = -1;
	}
}
