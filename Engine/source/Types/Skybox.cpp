#include "Types/Skybox.h"
#include "Render/Render.h"
#include "Logging/Logging.h"
#include "Types/Mesh.h"


kl::shaders Engine::Skybox::shaders = {};

Engine::Skybox::Skybox(const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) : Named(Named::Type::Skybox, name) {
	kl::dx::texture boxTex = Engine::gpu->newTexture(front, back, left, right, top, bottom);
	m_ShaderView = Engine::gpu->newShaderView(boxTex);
	Engine::gpu->destroy(boxTex);
}
Engine::Skybox::Skybox(const std::string& name, const kl::image& fullbox) : Named(Named::Type::Skybox) {
	if (fullbox.width() % 4 == 0 && fullbox.height() % 3 == 0) {
		const int partWidth = fullbox.width() / 4;
		const int partHeight = fullbox.height() / 3;
		if (partWidth == partHeight) {
			const kl::uint2 partSize(partWidth, partHeight);
			const kl::image front = fullbox.rect(partSize * kl::uint2(1, 1), partSize * kl::uint2(2, 2));
			const kl::image back = fullbox.rect(partSize * kl::uint2(3, 1), partSize * kl::uint2(4, 2));
			const kl::image left = fullbox.rect(partSize * kl::uint2(0, 1), partSize * kl::uint2(1, 2));
			const kl::image right = fullbox.rect(partSize * kl::uint2(2, 1), partSize * kl::uint2(3, 2));
			const kl::image top = fullbox.rect(partSize * kl::uint2(1, 0), partSize * kl::uint2(2, 1));
			const kl::image bottom = fullbox.rect(partSize * kl::uint2(1, 2), partSize * kl::uint2(2, 3));
			this->Skybox::Skybox(name, front, back, left, right, top, bottom);
		}
	}
	if (!m_ShaderView) {
		Engine::log("Texture file \"" + name + "\" has unsupported ratio!");
	}
}
Engine::Skybox::~Skybox() {
	if (Engine::gpu && isValid()) {
		Engine::gpu->destroy(m_ShaderView);
	}
}

bool Engine::Skybox::isValid() const {
	return bool(m_ShaderView);
}

void Engine::Skybox::render(const kl::mat4& vpMat) const {
	if (m_ShaderView) {
		Engine::gpu->bind(shaders);
		Engine::gpu->autoVertexCBuffer(vpMat);
		Engine::gpu->bindPixelShaderView(m_ShaderView, 0);
		Engine::gpu->draw(Engine::Meshes::Default::cube->buffer);
	}
}
