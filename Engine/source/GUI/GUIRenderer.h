#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "implot.h"
#include "KrimzLib.h"
#include "Scene/Scene.h"
#include "GUI/GUIRenderable.h"


namespace Krimz {
	class GUIRenderer {
		kl::ref<Scene> m_Scene;

	public:
		GUIRenderer();
		GUIRenderer(const GUIRenderer&) = delete;
		void operator=(const GUIRenderer&) = delete;
		~GUIRenderer();

		void initialize(kl::window& window);
		void initialize(kl::gpu& gpu);

		void loadKrimzTheme();

		void bind(kl::ref<Scene> scene);

		void render();
	};
}
