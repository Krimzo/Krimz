#pragma once

#include <functional>
#include <vector>

#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/buffer/sbuffer.h"
#include "KrimzLib/utility/convert.h"
#include "KrimzLib/math/float4.h"
#include "KrimzLib/math/mat4.h"
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/window/window.h"


namespace kl {
	class direct {
	public:
		kl::sbuffer* shadowMap = nullptr;
		kl::float4 color = kl::colors::white;
		kl::float3 direction = kl::float3(0.0f, -1.0f, -2.0f);
		float intensity = 1.0f;
		
		// Returns the true light color
		kl::float4 getCol() const;

		// Returns the direction of light
		kl::float3 getDir() const;

		// Returns the light vp matrix
		kl::mat4 matrix(const kl::camera& cam) const;
	};
}
