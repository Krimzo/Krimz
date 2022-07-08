#pragma once

#include "KrimzLib.h"
#include "Types/Named.h"


namespace Engine {
	class Skybox : public Named {
	private:
		kl::dx::view::shader m_ShaderView = nullptr;

	public:
		static kl::shaders shaders;

		Skybox(const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		Skybox(const std::string& name, const kl::image& fullbox);
		Skybox(const Engine::Skybox&) = delete;
		void operator=(const Engine::Skybox&) = delete;
		~Skybox();

		bool isValid() const;
		void render(const kl::mat4& vpMat) const;
	};

	inline std::list<kl::reference<Engine::Skybox>> skyboxes;
}
