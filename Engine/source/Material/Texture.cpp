#include "Material/Texture.h"


Krimz::Texture::Texture(const std::string& name, kl::ref<kl::gpu> gpu, const kl::image& image)
	: Named(Named::Type::Texture, name), m_GPU(gpu)
{
	kl::dx::texture texture = gpu->newTexture(image);
	view = gpu->newShaderView(texture);
	gpu->destroy(texture);
}
Krimz::Texture::~Texture()
{
	m_GPU->destroy(view);
}
