#include "Engine/Engine.h"


const float outlineFac = 1.025f;

void Outline() {
	// Saving the entity size
	const kl::float3 savedSize = selected->size;

	// Resizing the entity
	selected->size *= outlineFac;

	// Setting the vertex data
	kl::mat4 wvp = camera.matrix() * selected->matrix();
	outline_sh->setVertData(&wvp);
	
	// Setting the pixel data
	kl::float4 hig = outline;
	outline_sh->setPixlData(&hig);

	// Drawing the outline
	gpu->setDSState(kl::dbuffer::State::Mask);
	selected->render(false);
	gpu->setDSState(kl::dbuffer::State::Default);

	// Resetting the size
	selected->size = savedSize;
}
