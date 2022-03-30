#pragma once

#include <d3d11.h>
#include "KrimzLib.h"


namespace Engine { 
	class Texture {
	public:
		std::string name;
		ID3D11ShaderResourceView* view = nullptr;

		Texture(const std::string& name, ID3D11ShaderResourceView* view) {
			this->name = name;
			this->view = view;
		}
	};

	namespace Default {
		inline Engine::Texture* texture = nullptr;;
	}

	inline kl::pbuffer<Engine::Texture> textures;
}