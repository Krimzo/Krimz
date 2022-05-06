#include "Types/Material.h"


Engine::Material::Material() {}

bool Engine::Material::hasNormalMap() const {
	if (normalMap == Engine::Default::noneMap) {
		return false;
	}
	return true;
}

bool Engine::Material::hasRoughnessMap() const {
	if (roughnessMap == Engine::Default::noneMap) {
		return false;
	}
	return true;
}
