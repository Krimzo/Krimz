#pragma once

#include "Types/Light/Ambient.h"
#include "Types/Light/Direct.h"


namespace Engine::Light {
	inline Engine::Light::Ambient ambient = kl::float4(0.1f);
	inline kl::reference<Engine::Light::Direct> sun;
}
