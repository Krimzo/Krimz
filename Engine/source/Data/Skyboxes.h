#pragma once

#include "KrimzLib.h"
#include "Data/EObjects.h"


namespace Engine {
	class Skybox : public EObject {
	private:
		ID3D11VertexShader* sky_vtx = nullptr;
		ID3D11PixelShader* sky_pxl = nullptr;
		ID3D11Buffer* vtx_cb = nullptr;
		ID3D11Buffer* box_mes = nullptr;
		ID3D11ShaderResourceView* box_tex = nullptr;
		bool canDelete = true;
		bool valid = true;

	public:
		Skybox(const String& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		Skybox(const String& name, const kl::image& fullbox);
		Skybox(const Engine::Skybox& sb);
		~Skybox();

		// Valid
		bool isValid() const;

		// Render
		void render(const kl::mat4& vpMat) const;
	};

	inline std::list<Engine::Skybox> skyboxes;
	bool find(const std::list<Engine::Skybox>& skyboxes, const String& name);
}
