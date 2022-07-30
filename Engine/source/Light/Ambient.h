#pragma once

#include "KrimzLib.h"


namespace Krimz::Light {
	class Ambient {
		kl::float4 m_Color;

	public:
		Ambient();
		Ambient(const kl::color& color);

		kl::float4 color() const;
		void color(const kl::color& color);
	};
}
