#pragma once

#include <d3d11.h>
#include "KrimzLib.h"
#include "Data/EObjects.h"


namespace Engine { 
	class Texture : public EObject {
	public:
		ID3D11ShaderResourceView* view = nullptr;

		Texture(const std::string& name, ID3D11ShaderResourceView* view);
		~Texture();
	};

	namespace Default {
		inline Engine::Texture* texture = nullptr;
	}

	inline kl::pbuffer<Engine::Texture> textures;

	// Checks the buffer for the name
	bool find(const kl::pbuffer<Engine::Texture>& textures, const std::string& name);
}
