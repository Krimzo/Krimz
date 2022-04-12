#include "Data/Meshes.h"
#include "Render/Render.h"


void FixMeshName(std::string& name)
{
	const std::string nameCopy = name;
	int counter = 0;
	while (Engine::find(Engine::meshes, name))
		name = nameCopy + "_" + std::to_string(++counter);
}

Engine::Mesh::Mesh(const std::string& name, ID3D11Buffer* buff)
	: EObject::EObject(name), buff(buff)
{
	FixMeshName(this->name);
}
Engine::Mesh::~Mesh()
{
	Engine::Render::gpu->destroy(this->buff);
}

// Checks the buffer for the name
bool Engine::find(const kl::pbuffer<Engine::Mesh>& meshes, const std::string& name)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]->name == name)
			return true;
	}
	return false;
}
