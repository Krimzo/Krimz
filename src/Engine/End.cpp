#include "Engine/Engine.h"

#include "ImGui/imgui_impl_dx11.h"


void End() {
	// ImGui cleanup
	ImGui_ImplDX11_Shutdown();

	// Deleting entities
	entities.clear();

	// Deleting skyboxes
	skyboxes.clear();

	// Deleting the gpu
	delete gpu;
}
