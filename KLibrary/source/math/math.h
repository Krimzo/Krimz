#pragma once

#include <windows.h>
#include "math/float2.h"


namespace kl
{
	namespace math
	{
		// PI
		inline const float pi = 3.14159265358979f;

		// Returns x from the given y of the line that goes through points a and b
		float lineX(const kl::float2& a, const kl::float2& b, float y);

		// Returns y from the given x of the line that goes through points a and b
		float lineY(const kl::float2& a, const kl::float2& b, float x);

		// Normalizes value
		float norm(float value, float start, float end);
		float normAngle(float ang);
	};
}
