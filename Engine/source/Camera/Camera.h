#pragma once

#include "KrimzLib.h"
#include "Entity/Entity.h"
#include "Skybox/Skybox.h"
#include "Light/Ambient.h"


namespace Krimz
{
	class Camera : public kl::camera, public Entity
	{
	public:
		Light::Ambient ambient = {};
		kl::color color = { 20, 20, 20 };
		kl::ref<Skybox> skybox;

		Camera(const std::string& name);
		Camera(const Krimz::Camera&) = delete;
		void operator=(const Krimz::Camera&) = delete;
		~Camera();

		void render(kl::gpu& gpu) override;
		void gui_render() override;
	};
}
