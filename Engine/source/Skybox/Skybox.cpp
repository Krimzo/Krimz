#include "Skybox/Skybox.h"


Krimz::Skybox::Skybox(const std::string& name, kl::ref<kl::gpu> gpu, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom)
	: Entity(name), m_GPU(gpu) {
	m_Shaders = gpu->newShaders(kl::file::readString("source/Shaders/Skybox.hlsl"));
	kl::dx::texture boxTexture = gpu->newTexture(front, back, left, right, top, bottom);
	m_ShaderView = gpu->newShaderView(boxTexture);
	gpu->destroy(boxTexture);
}

Krimz::Skybox::Skybox(const std::string& name, kl::ref<kl::gpu> gpu, const kl::image& fullbox)
	: Entity(name), m_GPU(gpu) {
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
			this->Skybox::Skybox(name, gpu, front, back, left, right, top, bottom);
		}
	}
}

Krimz::Skybox::~Skybox() {
	if (m_ShaderView) {
		m_GPU->destroy(m_ShaderView);
	}
}

void Krimz::Skybox::render(kl::gpu& gpu) {
	if (m_ShaderView) {
		gpu.bind(m_Shaders);
		//gpu.autoVertexCBuffer();
		gpu.bindPixelShaderView(m_ShaderView, 0);
		//gpu.draw();
	}
}
