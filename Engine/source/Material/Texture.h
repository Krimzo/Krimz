#pragma once

#include "KrimzLib.h"
#include "Named/Named.h"


namespace Krimz {
	class Texture : public Named {
		kl::ref<kl::gpu> m_GPU;

	public:
		kl::dx::view::shader view = nullptr;

		Texture(const std::string& name, kl::ref<kl::gpu> gpu, const kl::image& image);
		Texture(const Krimz::Texture&) = delete;
		void operator=(const Krimz::Texture&) = delete;
		~Texture();
	};
}
