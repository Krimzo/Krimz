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
		std::vector<kl::ref<GUIRenderable>> m_Renderables;
		bool m_WinApiInitialized = false;
		bool m_DirectXInitalized = false;

	public:
		GUIRenderer();
		GUIRenderer(const GUIRenderer&) = delete;
		void operator=(const GUIRenderer&) = delete;
		~GUIRenderer();

		void initialize(kl::window& window);
		void initialize(kl::gpu& gpu);

		void loadKrimzTheme();

		void add(kl::ref<GUIRenderable> renderable);
		void remove(kl::ref<GUIRenderable> renderable);
		void clear();

		void render();
	};
}
