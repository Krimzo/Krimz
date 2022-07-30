#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "implot.h"
#include "KrimzLib.h"


namespace Krimz {
	namespace GUI {
		inline kl::dx::view::shader folderIcon = nullptr;
		inline kl::dx::view::shader folderEIcon = nullptr;
		inline kl::dx::view::shader fileIcon = nullptr;
		inline kl::dx::view::shader imageIcon = nullptr;
		inline kl::dx::view::shader objectIcon = nullptr;
		inline kl::dx::view::shader codeIcon = nullptr;
		inline kl::dx::view::shader scriptIcon = nullptr;
		inline kl::dx::view::shader solidRaIcon = nullptr;
		inline kl::dx::view::shader solidRaGIcon = nullptr;
		inline kl::dx::view::shader wireRaIcon = nullptr;
		inline kl::dx::view::shader wireRaGIcon = nullptr;

		inline bool viewportFocus = false;
		inline kl::uint2 viewportPosition = {};
		inline kl::uint2 viewportSize = 100;

		void Init();
		void Uninit();
		void LoadKrimzTheme();

		void MainMenuRender();
		void SceneRender();
		void ViewportRender();
		void PerformanceRender();
		void LogRender();
		void ExplorerRender();
		void PropertiesRender();

		namespace Scene {
			void Cameras();
			void Entites();
			void Meshes();
			void Skyboxes();
			void Textures();
		}
	}
}
