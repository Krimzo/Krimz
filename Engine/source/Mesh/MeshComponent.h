#pragma once

#include "Mesh/Mesh.h"
#include "Renderer/Renderable.h"
#include "GUI/GUIRenderable.h"


namespace Krimz {
	class MeshComponent : public Renderable, public GUIRenderable {
	public:
		kl::ref<Mesh> mesh;

		MeshComponent();
		MeshComponent(kl::ref<Mesh> Mesh);

		void render(kl::gpu& gpu) override;
		void gui_render() override;
	};
}
