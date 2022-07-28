#pragma once

#include "GUI/GUIRenderable.h"


namespace Krimz
{
	class GUISection : public GUIRenderable
	{
	public:
		GUISection();
		GUISection(const GUISection&) = delete;
		void operator=(const GUISection&) = delete;
		~GUISection();

		void gui_render() override;
	};
}
