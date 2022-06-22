#pragma once

#include "Render/Include/RenderTime.h"
#include "Render/Include/RenderLight.h"
#include "Render/Include/RenderRasters.h"
#include "Render/Include/RenderDepth.h"
#include "Render/Include/RenderShaders.h"
#include "Render/Include/RenderOutline.h"
#include "Render/Include/RenderColliders.h"


namespace Engine {
	inline kl::window window;
	inline std::unique_ptr<kl::gpu> gpu = nullptr;

	namespace Render {
		inline bool vSync = true;
		inline kl::uint2 targetSize = 100;
		inline kl::dx::view::target targetView = nullptr;
		inline kl::dx::view::shader shaderView = nullptr;

		void Shadows();
		void FixViewport();
		void Skybox();
		void Entities();
		void Outline();
		void Collider();
		void Gizmo();
		void GUI();
	}
}
