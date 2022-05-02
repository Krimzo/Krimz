#pragma once

#include "Data/Skyboxes.h"


namespace Engine {
	class Camera : public Named, public kl::camera {
	public:
		kl::color color = kl::color(20, 20, 20);
		Engine::Skybox* skybox = nullptr;

		Camera();
		Camera(const String& name);
		Camera(const Engine::Camera&) = delete;
		void operator=(const Engine::Camera&) = delete;
		~Camera();
	};

	namespace Default {
		inline std::unique_ptr<Engine::Camera> camera = nullptr;
	}

	inline std::list<std::shared_ptr<Engine::Camera>> cameras;
}
