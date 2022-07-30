#include "Light/Ambient.h"


Krimz::Light::Ambient::Ambient() : m_Color(0.1f, 0.1f, 0.1f, 0.1f)
{
}

Krimz::Light::Ambient::Ambient(const kl::color& color) : m_Color(color)
{
}

kl::float4 Krimz::Light::Ambient::color() const
{
	return m_Color;
}

void Krimz::Light::Ambient::color(const kl::color& color)
{
	m_Color = color;
}
