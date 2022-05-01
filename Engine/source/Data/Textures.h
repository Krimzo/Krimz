#pragma once

#include <d3d11.h>
#include "KrimzLib.h"
#include "Data/Named.h"


namespace Engine {
	class Texture : public Named {
	public:
		ID3D11ShaderResourceView* view = nullptr;

		Texture(const String& name, const kl::image& img);
		Texture(const Engine::Texture&) = delete;
		void operator=(const Engine::Texture&) = delete;
		~Texture();
	};

	namespace Default {
		inline Engine::Texture* texture = nullptr;
	}

	inline std::list<std::shared_ptr<Engine::Texture>> textures;
}
