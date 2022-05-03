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
		inline std::shared_ptr<Engine::Mesh> cube;
		inline std::shared_ptr<Engine::Mesh> sphere;
		inline std::shared_ptr<Engine::Mesh> capsule;
		inline std::shared_ptr<Engine::Mesh> pyramid;
		inline std::shared_ptr<Engine::Mesh> monke;
	}

	inline std::list<std::shared_ptr<Engine::Mesh>> meshes;
}
