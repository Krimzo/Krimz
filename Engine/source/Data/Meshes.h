#pragma once

#include "Data/Named.h"
#include "Physics/Physics.h"
#include "KrimzLib.h"


namespace Engine {
	class Mesh : public Named {
	public:
		std::vector<kl::vertex> vertices;
		ID3D11Buffer* buff = nullptr;
		physx::PxTriangleMesh* cooked = nullptr;

		Mesh(const String& name, const std::vector<kl::vertex>& vertices);
		Mesh(const Engine::Mesh&) = delete;
		void operator=(const Engine::Mesh&) = delete;
		~Mesh();
	};

	namespace Default {
		inline Engine::Mesh* cube = nullptr;
		inline Engine::Mesh* sphere = nullptr;
		inline Engine::Mesh* capsule = nullptr;
		inline Engine::Mesh* pyramid = nullptr;
		inline Engine::Mesh* monke = nullptr;
	}

	inline std::list<std::shared_ptr<Engine::Mesh>> meshes;
}
