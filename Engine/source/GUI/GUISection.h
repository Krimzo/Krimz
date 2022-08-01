#pragma once

#include "KrimzLib.h"
#include "GUI/GUIRenderable.h"


namespace Krimz {
	class GUISection : public GUIRenderable {
	public:
		std::function<bool()> start = []() { return false; };
		std::function<void()> render = []() {};
		std::function<void()> end = []() {};
		std::vector<kl::ref<GUIRenderable>> renderables;

		GUISection();

		void gui_render() override;
	};
}
