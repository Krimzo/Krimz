#pragma once

#include "Material/Material.h"
#include "Renderer/Renderable.h"
#include "GUI/GUIRenderable.h"


namespace Krimz
{
	class MaterialComponent : public Renderable, public GUIRenderable
	{
	public:
		kl::ref<Material> material;

		MaterialComponent();

		void render(kl::gpu& gpu) override;
		void gui_render() override;
	};
}
