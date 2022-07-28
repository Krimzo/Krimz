#pragma once

#include "Physics/Physical.h"
#include "GUI/GUIRenderable.h"


namespace Krimz
{
	class PhysicsComponent : public GUIRenderable
	{
	public:
		kl::ref<Physical> physical;

		PhysicsComponent();

		void gui_render() override;
	};
}
