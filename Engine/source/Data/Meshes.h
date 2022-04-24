#pragma once

#include "Data/EObjects.h"
#include "Physics/Physics.h"
#include "KrimzLib.h"


namespace Engine {
	class Mesh : public EObject {
	private:
		bool canDelete = true;

	public:
		std::vector<kl::vertex> vertices;
		ID3D11Buffer* buff = nullptr;
		physx::PxTriangleMesh* cooked = nullptr;

		Mesh(const String& name, const std::vector<kl::vertex>& vertices);
		Mesh(const Engine::Mesh& mesh);
		~Mesh();
	};

	namespace Default {
		inline Engine::Mesh* cube = nullptr;
		inline Engine::Mesh* sphere = nullptr;
		inline Engine::Mesh* capsule = nullptr;
		inline Engine::Mesh* pyramid = nullptr;
		inline Engine::Mesh* monke = nullptr;
	}

	inline std::list<Engine::Mesh> meshes;
	bool find(const std::list<Engine::Mesh>& meshes, const String& name);
}
