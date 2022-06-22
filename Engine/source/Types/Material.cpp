#include "Types/Material.h"


Engine::Material::Material() {}

bool Engine::Material::hasNormalMap() const {
	return normalMap != Engine::Textures::Default::nullMap;
}

bool Engine::Material::hasRoughnessMap() const {
	return roughnessMap != Engine::Textures::Default::nullMap;
}
