#include "Engine/Engine.h"


void Outline() {
	// Screen mesh
	static kl::mesh* screen = gpu->newMesh({
		kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
	});

	// Setting the index vertex data
	kl::mat4 wvp = camera.matrix() * selected->matrix();
	index_sh->setVertData(&wvp);

	// Setting the index pixel data
	kl::float4 indx = float(selectedInd);
	index_sh->setPixlData(&indx);

	// Clearing the outline index target
	kl::float4 clearInd = -1.0f;
	gpu->getCon()->ClearRenderTargetView(outlineBuff->getView(), (float*)&clearInd);

	// Drawing the selected
	gpu->bindTargets({ outlineBuff->getView() });
	selected->render(false);

	// Setting the outline pixel data
	OUTL_PS_CB pxlData = {};
	pxlData.higCol = outline;
	pxlData.selInd = float(selectedInd);
	outline_sh->setPixlData(&pxlData);

	// Render target reset
	gpu->bindInternal();

	// Binding the outline index tex
	outlineBuff->bind(0);

	// Drawing the outline
	gpu->setDSState(kl::dbuffer::State::Mask);
	screen->draw();
	gpu->setDSState(kl::dbuffer::State::Default);
}
