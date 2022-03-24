#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


namespace Engine {
	namespace GUI {
		// New ImGui frame
		void StartDraw();

		// ImGui data render
		void EndDraw();
	}
}
