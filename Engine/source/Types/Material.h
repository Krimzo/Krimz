#pragma once

#include "Types/Texture.h"


namespace Engine {
	class Material {
	public:
		float roughness = 0.5f;
		kl::reference<Engine::Texture> colorMap = Engine::Textures::Default::colorMap;
		kl::reference<Engine::Texture> normalMap = Engine::Textures::Default::nullMap;
		kl::reference<Engine::Texture> roughnessMap = Engine::Textures::Default::nullMap;

		Material();

		bool hasNormalMap() const;
		bool hasRoughnessMap() const;
	};
}
