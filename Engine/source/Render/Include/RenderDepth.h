#pragma once

#include "KrimzLib.h"


namespace Engine::DepthStencil {
	inline kl::dx::state::depth disabled = nullptr;
	inline kl::dx::state::depth depth = nullptr;
	inline kl::dx::state::depth write = nullptr;
	inline kl::dx::state::depth mask = nullptr;
}
