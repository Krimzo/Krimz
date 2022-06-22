#include "Types/Texture.h"
#include "Render/Render.h"


Engine::Texture::Texture(const std::string& name, const kl::image& img) : Named(Named::Type::Texture, name) {
	kl::dx::texture tex = Engine::gpu->newTexture(img);
	view = Engine::gpu->newShaderView(tex);
	Engine::gpu->destroy(tex);
}
Engine::Texture::~Texture() {
	if (Engine::gpu) {
		Engine::gpu->destroy(view);
	}
}
