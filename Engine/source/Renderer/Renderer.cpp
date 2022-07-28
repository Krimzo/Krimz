#include "Renderer/Renderer.h"


Krimz::Renderer::Renderer()
{

}

Krimz::Renderer::~Renderer()
{

}

kl::gpu& Krimz::Renderer::gpu()
{
	return *m_GPU;
}

void Krimz::Renderer::initalize(kl::window& window)
{
	m_GPU = kl::make<kl::gpu>(window);
}

void Krimz::Renderer::bind(kl::ref<Scene> scene)
{
	m_Scene = scene;
}

void Krimz::Renderer::color(const kl::color& color)
{
	m_ClearColor = color;
}

void Krimz::Renderer::clear()
{
	m_GPU->clear(m_ClearColor);
}

void Krimz::Renderer::render()
{
	if (m_Scene)
	{
		for (auto& entity : *m_Scene)
		{
			entity->render(*m_GPU);
		}
	}
}

void Krimz::Renderer::swap()
{
	m_GPU->swap(m_VSync);
}

bool Krimz::Renderer::vSync() const
{
	return m_VSync;
}
void Krimz::Renderer::vSync(bool vSync)
{
	m_VSync = vSync;
}
