#pragma once

#include "KrimzLib.h"


namespace Krimz {
	struct Transform {
		kl::float3 scale = {};
		kl::float3 rotation = {};
		kl::float3 position = {};

		Transform();
		Transform(const kl::float3& scale);
		Transform(const kl::float3& scale, const kl::float3& rotation);
		Transform(const kl::float3& scale, const kl::float3& rotation, const kl::float3& position);

		kl::mat4 matrix() const;
	};
}
