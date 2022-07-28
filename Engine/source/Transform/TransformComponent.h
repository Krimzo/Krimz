#pragma once

#include "Transform/Transform.h"
#include "GUI/GUIRenderable.h"


namespace Krimz
{
	class TransformComponent : public Transform, public GUIRenderable
	{
	public:
		TransformComponent();

		void gui_render() override;
	};
}
