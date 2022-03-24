#include "Engine/Render/Render.h"
#include "Engine/GUI/GUIStage.h"


void Engine::Render::GUIAfter() {
	Engine::GUI::ViewportOverlay();
}
