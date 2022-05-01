#include "Data/Meshes.h"
#include "Render/Render.h"


physx::PxTriangleMesh* CookMesh(const std::vector<kl::vertex>& vertData) {
	// Descriptor
	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = uint32_t(vertData.size());
	meshDesc.points.stride = uint32_t(sizeof(kl::vertex));
	meshDesc.points.data = &vertData[0];

	// Mesh cooking
	physx::PxDefaultMemoryOutputStream cookBuffer;
	Engine::Physics::cooking->cookTriangleMesh(meshDesc, cookBuffer);

	// Mesh creation
	physx::PxDefaultMemoryInputData createBuffer(cookBuffer.getData(), cookBuffer.getSize());
	return Engine::Physics::physics->createTriangleMesh(createBuffer);
}

Engine::Mesh::Mesh(const String& name, const std::vector<kl::vertex>& vertices) : Named(Named::Type::Mesh, name), vertices(vertices) {
	buff = Engine::Render::gpu->newVertBuffer(vertices);
	cooked = CookMesh(vertices);
}
Engine::Mesh::~Mesh() {
	Engine::Render::gpu->destroy(buff);
	cooked->release();
}
