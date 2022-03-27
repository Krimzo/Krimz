#pragma once

#include <d3d11.h>
#include "KrimzLib/memory/pbuffer.h"


namespace Engine {
	class Mesh {
	public:
		std::string name;
		ID3D11Buffer* buff = nullptr;

		Mesh(const std::string& name, ID3D11Buffer* buff) {
			this->name = name;
			this->buff = buff;
		}
	};

	namespace Default {
		inline Engine::Mesh* mesh = nullptr;
	}

	namespace Data {
		inline kl::pbuffer<Engine::Mesh> meshes;
	}
}
