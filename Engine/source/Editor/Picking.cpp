#include "Editor/Picking.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void Krimz::Picking::ReadObjectIndex()
{
	const kl::uint2 indexPos = Krimz::window.mouse.position - Krimz::GUI::viewportPosition;
	if (indexPos.x >= 0 && indexPos.x < Krimz::GUI::viewportSize.x && indexPos.y >= 0 && indexPos.y < Krimz::GUI::viewportSize.y)
	{
		D3D11_BOX srcBox = {};
		srcBox.left = indexPos.x;
		srcBox.right = srcBox.left + 1;
		srcBox.top = indexPos.y;
		srcBox.bottom = srcBox.top + 1;
		srcBox.front = 0;
		srcBox.back = 1;
		Krimz::gpu->con()->CopySubresourceRegion(Krimz::Picking::textureStaging, 0, 0, 0, 0, Krimz::Picking::texture, 0, &srcBox);

		D3D11_MAPPED_SUBRESOURCE msr = {};
		Krimz::gpu->con()->Map(Krimz::Picking::textureStaging, 0, D3D11_MAP_READ, NULL, &msr);

		float index = 0;
		memcpy(&index, msr.pData, sizeof(float));

		Krimz::gpu->con()->Unmap(Krimz::Picking::textureStaging, NULL);
		Krimz::Picking::mouseIndex = int(index);
	}
	else
	{
		Krimz::Picking::mouseIndex = -1;
	}
}
