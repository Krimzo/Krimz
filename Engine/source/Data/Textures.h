#pragma once

#include <d3d11.h>
#include "KrimzLib.h"
#include "Data/EObjects.h"


namespace Engine {
	class Texture : public EObject {
	private:
		bool canDelete = true;

	public:
		ID3D11ShaderResourceView* view = nullptr;

		Texture(const std::string& name, const kl::image& img);
		Texture(const Engine::Texture& tex);
		~Texture();
	};

	namespace Default {
		inline Engine::Texture* texture = nullptr;
	}

	inline std::list<Engine::Texture> textures;
	bool find(const std::list<Engine::Texture>& textures, const std::string& name);
}
