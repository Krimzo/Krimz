#include "Types/Mesh.h"
#include "Render/Render.h"


physx::PxTriangleMesh* CookMesh(const std::vector<kl::vertex>& vertData) {
	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = uint(vertData.size());
	meshDesc.points.stride = uint(sizeof(kl::vertex));
	meshDesc.points.data = &vertData[0];

	physx::PxDefaultMemoryOutputStream cookBuffer;
	Engine::Physics::cooking->cookTriangleMesh(meshDesc, cookBuffer);

	physx::PxDefaultMemoryInputData createBuffer(cookBuffer.getData(), cookBuffer.getSize());
	return Engine::Physics::physics->createTriangleMesh(createBuffer);
}

Engine::Mesh::Mesh(const std::string& name, const std::vector<kl::vertex>& vertices) : Named(Named::Type::Mesh, name), vertices(vertices) {
	buffer = Engine::gpu->newVertexBuffer(vertices);
	cooked = CookMesh(vertices);
}
Engine::Mesh::~Mesh() {
	if (Engine::gpu) {
		Engine::gpu->destroy(buffer);
	}
	if (Engine::Physics::physics) {
		cooked->release();
	}
}
