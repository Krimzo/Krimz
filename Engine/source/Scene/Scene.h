#pragma once

#include "Entity/Entity.h"


namespace Krimz {
	class Scene : public std::vector<kl::ref<Entity>> {
	public:
		Scene();
		Scene(const Scene&) = delete;
		void operator=(const Scene&) = delete;
		~Scene();

		bool toFile(const std::string& filePath) const;
		bool fromFile(const std::string& filePath);
	};
}
