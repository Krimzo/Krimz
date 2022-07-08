#pragma once

#include "Types/Skybox.h"


namespace Engine {
	class Camera : public Named, public kl::camera {
	public:
		kl::color color = kl::color(20, 20, 20);
		kl::reference<Engine::Skybox> skybox;

		Camera();
		Camera(const std::string& name);
		Camera(const Engine::Camera&) = delete;
		void operator=(const Engine::Camera&) = delete;
	};

	namespace Default {
		inline kl::reference<Engine::Camera> camera;
	}

	namespace Selected {
		inline kl::reference<Engine::Camera> camera;
	}

	inline std::list<kl::reference<Engine::Camera>> cameras;
}
