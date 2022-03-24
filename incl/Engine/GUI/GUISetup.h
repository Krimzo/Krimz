#pragma once


#include <functional>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


namespace Engine {
	namespace GUI {
		// Inits the ImGui context
		void Init();

		// Destroys the ImGui context
		void Uninit();

		// Loads my custom theme
		void LoadKrimzTheme();
	}
}
