#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


namespace Engine {
	namespace GUI {
		// Before funcs
		void MainMenu();
		void Scene();
		void Explorer();
		void Properties();

		// After funcs
		void ViewportOverlay();
	}
}
