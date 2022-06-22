#pragma once

#include "Types/Texture.h"


namespace Engine {
	class Material {
	public:
		float roughness = 0.5f;
		std::shared_ptr<Engine::Texture> colorMap = Engine::Textures::Default::colorMap;
		std::shared_ptr<Engine::Texture> normalMap = Engine::Textures::Default::nullMap;
		std::shared_ptr<Engine::Texture> roughnessMap = Engine::Textures::Default::nullMap;

		Material();

		bool hasNormalMap() const;
		bool hasRoughnessMap() const;
	};
}
