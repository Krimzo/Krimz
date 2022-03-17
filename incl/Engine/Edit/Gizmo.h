#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Gizmo {
		// Types
		enum Type {
			NONE = 0,
			SCALE,
			MOVE,
			ROTATE
		};

		// Selected gizmo
		inline int selected = Type::NONE;

		// Global gizmo scale
		inline const float scale = 0.25f;

		// Gizmo meshes
		inline ID3D11Buffer* scaleM = nullptr;
		inline ID3D11Buffer* moveM = nullptr;
		inline ID3D11Buffer* rotateM = nullptr;

		// Gizmo colors
		inline kl::float4 colX = kl::color(205, 55, 75);
		inline kl::float4 colY = kl::color(115, 175, 40);
		inline kl::float4 colZ = kl::color(55, 120, 205);
	}
}
