#pragma once

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
	class ray {
	public:
		kl::vec3 origin;
		kl::vec3 direction;

		// Constructors
		kl::ray();
		kl::ray(const kl::vec3& origin, const kl::vec3& direction);

		// Returns an intersection with a plane
		kl::vec3 intersect(const kl::vec3& planeNorm, const kl::vec3& planePos) const;
	};
}
