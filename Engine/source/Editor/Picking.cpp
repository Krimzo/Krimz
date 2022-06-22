#include "Editor/Picking.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void Engine::Picking::ReadObjectIndex() {
	const kl::uint2 indexPos = Engine::window.mouse.position - Engine::GUI::viewportPosition;
	if (indexPos.x >= 0 && indexPos.x < Engine::GUI::viewportSize.x && indexPos.y >= 0 && indexPos.y < Engine::GUI::viewportSize.y) {
		D3D11_BOX srcBox = {};
		srcBox.left = indexPos.x;
		srcBox.right = srcBox.left + 1;
		srcBox.top = indexPos.y;
		srcBox.bottom = srcBox.top + 1;
		srcBox.front = 0;
		srcBox.back = 1;
		Engine::gpu->con()->CopySubresourceRegion(Engine::Picking::textureStaging, 0, 0, 0, 0, Engine::Picking::texture, 0, &srcBox);

		D3D11_MAPPED_SUBRESOURCE msr = {};
		Engine::gpu->con()->Map(Engine::Picking::textureStaging, 0, D3D11_MAP_READ, NULL, &msr);

		float index = 0;
		memcpy(&index, msr.pData, sizeof(float));

		Engine::gpu->con()->Unmap(Engine::Picking::textureStaging, NULL);
		Engine::Picking::mouseIndex = int(index);
	}
	else {
		Engine::Picking::mouseIndex = -1;
	}
}
