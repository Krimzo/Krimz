#include "Engine/Engine.h"


void Outline() {
	// Screen mesh
	static kl::mesh* screen = gpu->newMesh({
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
	});	
	
	// Setting the pixel data
	OUTL_PS_CB pxlData = {};
	pxlData.higCol = outline;
	pxlData.selInd = float(selectedInd);
	outline_sh->setPixlData(&pxlData);

	// Binding the index texture
	gpu->bindIndRes(0);

	// Drawing the outline
	gpu->setDSState(kl::dbuffer::State::Mask);
	screen->draw();
	gpu->setDSState(kl::dbuffer::State::Default);

	// Render target reset
	gpu->bindInternal();
}
