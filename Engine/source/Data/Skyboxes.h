#pragma once

#include "KrimzLib.h"
#include "Data/Named.h"


namespace Engine {
	class Skybox : public Named {
	private:
		ID3D11VertexShader* sky_vtx = nullptr;
		ID3D11PixelShader* sky_pxl = nullptr;
		ID3D11Buffer* vtx_cb = nullptr;
		ID3D11Buffer* box_mes = nullptr;
		ID3D11ShaderResourceView* box_tex = nullptr;

	public:
		Skybox(const String& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		Skybox(const String& name, const kl::image& fullbox);
		Skybox(const Engine::Skybox&) = delete;
		void operator=(const Engine::Skybox&) = delete;
		~Skybox();

		bool isValid() const;
		void render(const kl::mat4& vpMat) const;
	};

	inline std::list<std::shared_ptr<Engine::Skybox>> skyboxes;
}
