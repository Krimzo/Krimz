#pragma once

#include "Scripter/Script.h"
#include "GUI/GUIRenderable.h"


namespace Krimz {
	class ScriptComponent : public GUIRenderable {
	public:
		kl::ref<Script> script;

		ScriptComponent();

		void gui_render() override;
	};
}
