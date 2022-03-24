#include "Engine/GUI/GUIStage.h"
#include "Engine/GUI/GUIProperties.h"
#include "Engine/Window/Window.h"


void Engine::GUI::Explorer() {
	// Window pos/size calc
	Engine::GUI::explorPos = kl::int2(Engine::GUI::sceneSize.x, int(Engine::Window::size.y * (1.0f - Engine::GUI::explorerRatio)));
	Engine::GUI::explorSize = kl::int2(int(Engine::Window::size.x - 2.0f * Engine::GUI::sceneSize.x), int(Engine::Window::size.y * Engine::GUI::explorerRatio + 1.0f));

	// Window pos/size set
	ImGui::SetNextWindowPos(ImVec2(float(Engine::GUI::explorPos.x), float(Engine::GUI::explorPos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(Engine::GUI::explorSize.x), float(Engine::GUI::explorSize.y)));

	// Window draw
	if (ImGui::Begin("File Explorer", nullptr, Engine::GUI::panelFlags)) {

		// End draw
		ImGui::End();
	}
}
