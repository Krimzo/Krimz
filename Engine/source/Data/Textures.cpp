#include "Data/Textures.h"
#include "Render/Render.h"


void FixTextureName(std::string& name) {
	const std::string nameCopy = name;
	int counter = 0;
	while (Engine::find(Engine::textures, name)) {
		name = nameCopy + "_" + std::to_string(++counter);
	}
}

Engine::Texture::Texture(const std::string& name, const kl::image& img) : EObject(name) {
	FixTextureName(this->name);
	ID3D11Texture2D* tex = Engine::Render::gpu->newTexture(img);
	view = Engine::Render::gpu->newShaderView(tex);
	Engine::Render::gpu->destroy(tex);
}
Engine::Texture::Texture(const Engine::Texture& tex) : EObject(tex.name) {
	view = tex.view;
	((Engine::Texture*)&tex)->canDelete = false;
	canDelete = true;
}
Engine::Texture::~Texture() {
	if (canDelete) {
		Engine::Render::gpu->destroy(view);
	}
}

// Checks the buffer for the name
bool Engine::find(const std::list<Engine::Texture>& textures, const std::string& name) {
	if (Engine::Default::texture && name == "default") {
		return true;
	}
	for (auto& tex : textures) {
		if (tex.name == name) {
			return true;
		}
	}
	return false;
}
