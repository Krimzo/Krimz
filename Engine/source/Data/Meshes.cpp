#include "Data/Meshes.h"
#include "Render/Render.h"


void FixMeshName(String& name) {
	const String nameCopy = name;
	int counter = 0;
	while (Engine::find(Engine::meshes, name)) {
		name = nameCopy + "_" + std::to_string(++counter);
	}
}

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

Engine::Mesh::Mesh(const String& name, const std::vector<kl::vertex>& vertices) : Named(name), vertices(vertices) {
	FixMeshName(this->name);
	buff = Engine::Render::gpu->newVertBuffer(vertices);
	cooked = CookMesh(vertices);
}
Engine::Mesh::Mesh(const Engine::Mesh& mesh) : Named(mesh.name) {
	vertices = mesh.vertices;
	buff = mesh.buff;
	cooked = mesh.cooked;
	((Engine::Mesh*)&mesh)->canDelete = false;
	canDelete = true;
}
Engine::Mesh::~Mesh() {
	if (canDelete) {
		Engine::Render::gpu->destroy(buff);
		cooked->release();
	}
}

// Checks the buffer for the name
bool Engine::find(const std::list<Engine::Mesh>& meshes, const String& name) {
	if ((Engine::Default::cube && name == "cube") || (Engine::Default::sphere && name == "sphere") || (Engine::Default::capsule && name == "capsule") || (Engine::Default::pyramid && name == "pyramid") || (Engine::Default::monke && name == "monke")) {
		return true;
	}
	for (auto& mes : meshes) {
		if (mes.name == name) {
			return true;
		}
	}
	return false;
}
