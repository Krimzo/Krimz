#include "Types/Texture.h"
#include "Render/Render.h"


Engine::Texture::Texture(const String& name, const kl::image& img) : Named(Named::Type::Texture, name) {
	ID3D11Texture2D* tex = Engine::Render::gpu->newTexture(img);
	view = Engine::Render::gpu->newShaderView(tex);
	Engine::Render::gpu->destroy(tex);
}
Engine::Texture::~Texture() {
	if (Engine::Render::gpu) {
		Engine::Render::gpu->destroy(view);
	}
}
