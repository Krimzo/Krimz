#pragma once

#include "Data/Named.h"
#include "KrimzLib.h"


namespace Engine {
	class Camera : public Named, public kl::camera {
	public:
		Camera();
		Camera(const String& name);
		Camera(const Engine::Camera&) = delete;
		void operator=(const Engine::Camera&) = delete;
		~Camera();
	};

	inline std::list<std::shared_ptr<Engine::Camera>> cameras;
}
