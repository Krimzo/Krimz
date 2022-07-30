#pragma once

#include "KrimzLib.h"


namespace Krimz {
	namespace Gizmo {
		enum class Type {
			NONE = 0,
			SCALE,
			MOVE,
			ROTATE
		};

		inline const float scale = 0.25f;

		inline kl::vector3<kl::float4> colors = {
			kl::color(205, 055, 075),
			kl::color(115, 175, 40),
			kl::color(055, 120, 205)
		};
	}
	namespace Selected {
		inline Gizmo::Type gizmo = Gizmo::Type::NONE;
	}
}
