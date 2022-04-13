#pragma once

#include "KrimzLib.h"
#include "Data/EObjects.h"


namespace Engine
{
	class skybox : public EObject
	{
	private:
		// DirectX pointers
		kl::gpu* gpu = nullptr;
		ID3D11VertexShader* sky_vtx = nullptr;
		ID3D11PixelShader* sky_pxl = nullptr;
		ID3D11Buffer* vtx_cb = nullptr;
		ID3D11Buffer* box_mes = nullptr;
		ID3D11ShaderResourceView* box_tex = nullptr;

		// Constr
		skybox(kl::gpu* gpu, const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);

	public:
		// Getters
		static Engine::skybox* newSkybox(kl::gpu* gpu, const std::string& name, const kl::image& fullbox);
		static Engine::skybox* newSkybox(kl::gpu* gpu, const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);

		// Destr
		skybox(const Engine::skybox&) = delete;
		~skybox();

		// Renders the cubemap
		void render(const kl::mat4& vpMat) const;
	};

	inline kl::pbuffer<Engine::skybox> skyboxes;
	bool find(const kl::pbuffer<Engine::skybox>& skyboxes, const std::string& name);
}
