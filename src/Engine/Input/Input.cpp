#include "Engine/Input.h"


// Returns the picking index
int GetIndex(const kl::int2& pos) {
    // Checking if the pos is in frame
    if (pos.x >= 0 && pos.x < win.getSize().x && pos.y >= 0 && pos.y < win.getSize().y) {
        // Copying the index texture pixel
        D3D11_BOX srcBox = {};
        srcBox.left = pos.x;
        srcBox.right = srcBox.left + 1;
        srcBox.top = pos.y;
        srcBox.bottom = srcBox.top + 1;
        srcBox.front = 0;
        srcBox.back = 1;
        gpu->con()->CopySubresourceRegion(pickingTexST, 0, 0, 0, 0, pickingTex, 0, &srcBox);

        // Mapping the staging texture
        D3D11_MAPPED_SUBRESOURCE msr = {};
        gpu->con()->Map(pickingTexST, 0, D3D11_MAP_READ, NULL, &msr);

        // Reading the bytes
        float index = 0;
        memcpy(&index, msr.pData, sizeof(float));

        // Unmapping the staging texture
        gpu->con()->Unmap(pickingTexST, NULL);

        // Getting the index
        return int(index);
    }
    return -1;
}

void Input() {
	// Entity index getter
	entityIndex = GetIndex(win.mouse.position);
	
	// LMB
	LMBInput();

	// Gizmo highlight
	GizmoHigh();

	// Gizmo input
	GizmoInput();

	// Free flight cam
	FreeCam();
}
