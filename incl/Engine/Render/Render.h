#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Render {
		inline kl::gpu* gpu = nullptr;
		inline kl::camera camera;

		void Shadows();
		void Entity();
		void Outline();
		void Gizmo();
		void GUI();
	}
}
