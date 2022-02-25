#include "KrimzLib/vecmath/ray.h"


// Constructors
kl::ray::ray() {

}
kl::ray::ray(const kl::vec3& origin, const kl::vec3& direction) {
	this->origin = origin;
	this->direction = direction;
}

// Returns an intersection with a plane
kl::vec3 kl::ray::intersect(const kl::plane& plane) const {
	return this->origin - this->direction * ((this->origin - plane.point).dot(plane.normal) / this->direction.dot(plane.normal));
}
