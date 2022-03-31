#pragma once

#include <d3d11.h>
#include "KrimzLib.h"
#include "Data/EObjects.h"


namespace Engine {
	class Mesh : public EObject {
	public:
		ID3D11Buffer* buff = nullptr;

		Mesh(const std::string& name, ID3D11Buffer* buff);
		~Mesh();
	};

	namespace Default {
		inline Engine::Mesh* mesh = nullptr;
	}

	inline kl::pbuffer<Engine::Mesh> meshes;

	// Checks the buffer for the name
	bool find(const kl::pbuffer<Engine::Mesh>& meshes, const std::string& name);
}
