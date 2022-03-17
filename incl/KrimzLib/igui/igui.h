#pragma once

#ifdef KL_USING_IMGUI

#include <functional>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


namespace kl {
	namespace igui {
		// Inits the ImGui context
		void init();

		// Destroys the ImGui context
		void uninit();

		// Loads my custom theme
		void loadKrimzTheme();

		// New ImGui frame
		void startDraw();

		// ImGui data render
		void endDraw();
	}
}

#endif
