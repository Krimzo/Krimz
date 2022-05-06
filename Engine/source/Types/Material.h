#pragma once

#include "Types/Texture.h"


namespace Engine {
	class Material {
	public:
		float roughness = 0.5f;
		std::shared_ptr<Engine::Texture> colorMap = Engine::Default::colorMap;
		std::shared_ptr<Engine::Texture> normalMap = Engine::Default::noneMap;
		std::shared_ptr<Engine::Texture> roughnessMap = Engine::Default::noneMap;

		Material();

		bool hasNormalMap() const;
		bool hasRoughnessMap() const;
	};
}
