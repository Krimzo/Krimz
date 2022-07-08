#pragma once

#include <d3d11.h>
#include "KrimzLib.h"
#include "Types/Named.h"


namespace Engine {
	class Texture : public Named {
	public:
		kl::dx::view::shader view = nullptr;

		Texture(const std::string& name, const kl::image& img);
		Texture(const Engine::Texture&) = delete;
		void operator=(const Engine::Texture&) = delete;
		~Texture();
	};

	namespace Textures {
		namespace Default {
			inline kl::reference<Engine::Texture> colorMap;
			inline kl::reference<Engine::Texture> nullMap;
		}
	}

	inline std::list<kl::reference<Engine::Texture>> textures;
}
