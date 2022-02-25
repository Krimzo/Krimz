#include "KrimzLib/vecmath/ray.h"


// Constructors
kl::ray::ray() {

}
kl::ray::ray(const kl::vec3& origin, const kl::vec3& direction) {
	this->origin = origin;
	this->direction = direction;
}

// Returns an intersection with a plane
kl::vec3 kl::ray::intersect(const kl::vec3& planeNorm, const kl::vec3& planePos) const {
	// Dot product check
	const float nrDot = planeNorm.dot(this->direction);
	if (nrDot != 0) {
		// Plane calculation
		const float d = planeNorm.dot(planePos);

		// Compute the ray t
		const float t = (d - planeNorm.dot(this->origin)) / nrDot;

		// Intersect test
		if (t >= 0.0f && t <= 1.0f) {
			return this->origin + this->direction * t;
		}
	}
	return kl::vec3(0.0f);
}
