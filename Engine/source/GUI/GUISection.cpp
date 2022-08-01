#include "GUI/GUISection.h"


Krimz::GUISection::GUISection() {}

void Krimz::GUISection::gui_render() {
	if (start()) {
		render();
		for (auto& renderable : renderables) {
			renderable->gui_render();
		}
		end();
	}
}
