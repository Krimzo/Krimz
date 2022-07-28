#include "Transform/Transform.h"


Krimz::Transform::Transform()
{
}

Krimz::Transform::Transform(const kl::float3& scale) : scale(scale)
{
}

Krimz::Transform::Transform(const kl::float3& scale, const kl::float3& rotation) : scale(scale), rotation(rotation)
{
}

Krimz::Transform::Transform(const kl::float3& scale, const kl::float3& rotation, const kl::float3& position) : scale(scale), rotation(rotation), position(position)
{
}

kl::mat4 Krimz::Transform::matrix() const
{
	return kl::mat4::translation(position) * kl::mat4::rotation(rotation) * kl::mat4::scaling(scale);
}
