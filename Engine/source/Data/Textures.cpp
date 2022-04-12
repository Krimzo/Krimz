#include "Data/Textures.h"
#include "Render/Render.h"


void FixTextureName(std::string& name)
{
	const std::string nameCopy = name;
	int counter = 0;
	while (Engine::find(Engine::textures, name))
		name = nameCopy + "_" + std::to_string(++counter);
}

Engine::Texture::Texture(const std::string& name, ID3D11ShaderResourceView* view)
	: EObject::EObject(name), view(view)
{
	FixTextureName(this->name);
}
Engine::Texture::~Texture()
{
	Engine::Render::gpu->destroy(this->view);
}

// Checks the buffer for the name
bool Engine::find(const kl::pbuffer<Engine::Texture>& textures, const std::string& name)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i]->name == name)
			return true;
	}
	return false;
}
