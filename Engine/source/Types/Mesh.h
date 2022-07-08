#pragma once

#include "Types/Named.h"
#include "Physics/Physics.h"
#include "KrimzLib.h"


namespace Engine {
	class Mesh : public Named {
	public:
		std::vector<kl::vertex> vertices;
		kl::dx::buffer buffer = nullptr;
		physx::PxTriangleMesh* cooked = nullptr;

		Mesh(const std::string& name, const std::vector<kl::vertex>& vertices);
		Mesh(const Engine::Mesh&) = delete;
		void operator=(const Engine::Mesh&) = delete;
		~Mesh();
	};

	namespace Meshes {
		namespace Util {
			inline kl::reference<Engine::Mesh> screen;
		}

		namespace Default {
			inline kl::reference<Engine::Mesh> cube;
			inline kl::reference<Engine::Mesh> sphere;
			inline kl::reference<Engine::Mesh> capsule;
			inline kl::reference<Engine::Mesh> pyramid;
			inline kl::reference<Engine::Mesh> monke;
		}

		namespace Gizmo {
			inline kl::dx::buffer scale = nullptr;
			inline kl::dx::buffer move = nullptr;
			inline kl::dx::buffer rotate = nullptr;
		}
	}

	inline std::list<kl::reference<Engine::Mesh>> meshes;
}
