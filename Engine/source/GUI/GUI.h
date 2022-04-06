#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "implot.h"
#include "KrimzLib.h"


namespace Engine
{
	namespace GUI
	{
		// Icon textures
		inline ID3D11ShaderResourceView* folderIcon = nullptr;
		inline ID3D11ShaderResourceView* folderEIcon = nullptr;
		inline ID3D11ShaderResourceView* fileIcon = nullptr;
		inline ID3D11ShaderResourceView* imageIcon = nullptr;
		inline ID3D11ShaderResourceView* objectIcon = nullptr;
		inline ID3D11ShaderResourceView* codeIcon = nullptr;
		inline ID3D11ShaderResourceView* scriptIcon = nullptr;

		// Viewport
		inline bool viewportFocus = false;
		inline kl::int2 viewportPos;
		inline kl::int2 viewportSize;

		// Setup
		void Init();
		void Uninit();
		void LoadKrimzTheme();

		// Stages
		void MainMenu();
		void Scene();
		void Viewport();
		void Performance();
		void Log();
		void Explorer();
		void Properties();
	}
}
