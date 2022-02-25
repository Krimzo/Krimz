#pragma once

#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/plane.h"


namespace kl {
	class ray {
	public:
		kl::vec3 origin;
		kl::vec3 direction;

		// Constructors
		kl::ray();
		kl::ray(const kl::vec3& origin, const kl::vec3& direction);

		// Returns an intersection with a plane
		kl::vec3 intersect(const kl::plane& plane) const;
	};
}
