#pragma once

#include "KrimzLib.h"
#include "Renderer/Renderable.h"
#include "Scene/Scene.h"


namespace Krimz
{
	class Renderer
	{
		kl::float4 m_ClearColor = kl::colors::black;
		bool m_VSync = true;

		kl::ref<kl::gpu> m_GPU;
		kl::ref<Scene> m_Scene;

	public:
		Renderer();
		Renderer(const Renderer&) = delete;
		void operator=(const Renderer&) = delete;
		~Renderer();

		kl::gpu& gpu();

		void initalize(kl::window& window);
		void bind(kl::ref<Scene> scene);

		void color(const kl::color& color);

		void clear();
		void render();
		void swap();

		bool vSync() const;
		void vSync(bool vSync);
	};
}
