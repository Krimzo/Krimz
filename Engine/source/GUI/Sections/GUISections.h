#pragma once

#include "KrimzLib.h"
#include "GUI/GUISection.h"
#include "GUI/GUIRenderer.h"


namespace Krimz {
	namespace GUISections {
		kl::ref<GUISection> MainMenu();
		kl::ref<GUISection> Scene();
		kl::ref<GUISection> Viewport();
		kl::ref<GUISection> Explorer();
		kl::ref<GUISection> LogView();
		kl::ref<GUISection> Performance();
		kl::ref<GUISection> Properties();
	}

	void LoadGuiSections(GUIRenderer& renderer);
}
