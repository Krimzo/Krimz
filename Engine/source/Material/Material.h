#pragma once

#include "Material/Texture.h"


namespace Krimz {
	class Material {
	public:
		float roughness = 0.5f;
		kl::ref<Texture> colorMap;
		kl::ref<Texture> normalMap;
		kl::ref<Texture> roughnessMap;

		Material();
	};
}
