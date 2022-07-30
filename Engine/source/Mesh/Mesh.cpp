#include "Mesh/Mesh.h"
#include "Render/Render.h"


static physx::PxTriangleMesh* CookMesh(PxCooking* cooking, PxPhysics* physics, const std::vector<kl::vertex>& vertices) {
	PxTriangleMeshDesc meshDescriptor;
	meshDescriptor.points.count = uint(vertices.size());
	meshDescriptor.points.stride = uint(sizeof(kl::vertex));
	meshDescriptor.points.data = vertices.data();

	PxDefaultMemoryOutputStream cookBuffer;
	cooking->cookTriangleMesh(meshDescriptor, cookBuffer);

	PxDefaultMemoryInputData cookedBuffer(cookBuffer.getData(), cookBuffer.getSize());
	return physics->createTriangleMesh(cookedBuffer);
}

Krimz::Mesh::Mesh(const std::string& name, kl::ref<kl::gpu> gpu, PxCooking* cooking, PxPhysics* physics, const std::vector<kl::vertex>& vertices) : Named(Named::Type::Mesh, name), m_GPU(gpu), vertices(vertices) {
	buffer = gpu->newVertexBuffer(vertices);
	cooked = CookMesh(cooking, physics, vertices);
}
Krimz::Mesh::~Mesh() {
	m_GPU->destroy(buffer);
	cooked->release();
}
