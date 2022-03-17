#pragma once

#include "Engine/Engine.h"


namespace Engine {
	namespace GUI {
		// Panel flags
		inline const int panelFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

		// Panel sizes
		inline const float panelRatio = 0.18f;
		inline const float explorerRatio = 0.20f;

		// Main menu
		inline ImVec2 mMenuSize;

		// Scene
		inline kl::int2 scenePos;
		inline kl::int2 sceneSize;

		// Explorer
		inline kl::int2 explorPos;
		inline kl::int2 explorSize;

		// Properties
		inline kl::int2 propsPos;
		inline kl::int2 propsSize;

		// Viewport
		inline bool viewportFocus = false;
		inline kl::int2 viewportPos;
		inline kl::int2 viewportSize;

		// Before funcs
		void MainMenu();
		void Scene();
		void Explorer();
		void Properties();

		// After funcs
		void ViewportOverlay();
	}
}
