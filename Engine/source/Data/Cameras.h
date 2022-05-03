#pragma once

#include "Data/Skyboxes.h"


namespace Engine {
	class Camera : public Named, public kl::camera {
	public:
		kl::color color = kl::color(20, 20, 20);
		std::shared_ptr<Engine::Skybox> skybox;

		Camera();
		Camera(const String& name);
		Camera(const Engine::Camera&) = delete;
		void operator=(const Engine::Camera&) = delete;
	};

	namespace Default {
		inline std::shared_ptr<Engine::Camera> camera;
	}

	namespace Selected {
		inline std::shared_ptr<Engine::Camera> camera;
	}

	inline std::list<std::shared_ptr<Engine::Camera>> cameras;
}
