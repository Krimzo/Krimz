#pragma once

#include "KrimzLib.h"
#include "Entity/Entity.h"


namespace Krimz
{
	class Skybox : public Entity
	{
		kl::ref<kl::gpu> m_GPU;
		kl::shaders m_Shaders;
		kl::dx::view::shader m_ShaderView = nullptr;

	public:
		Skybox(const std::string& name, kl::ref<kl::gpu> gpu, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		Skybox(const std::string& name, kl::ref<kl::gpu> gpu, const kl::image& fullbox);
		Skybox(const Krimz::Skybox&) = delete;
		void operator=(const Krimz::Skybox&) = delete;
		~Skybox();

		void render(kl::gpu& gpu) override;
	};
}
