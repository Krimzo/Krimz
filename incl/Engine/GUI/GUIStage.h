#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


namespace Engine {
	namespace GUI {
		void MainMenu();
		void Scene();
		void Viewport();
		void Explorer();
		void Performance();
		void Log();
		void Properties();
	}
}
