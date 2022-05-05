#pragma once

#include "KrimzLib.h"


namespace Engine {
	class Named {
	public:
		enum class Type {
			Entity = 0,
			Mesh,
			Texture,
			Skybox,
			Camera
		};

	private:
		String name;
		Type type;

	public:
		Named(Named::Type type);
		Named(Named::Type type, const String& name);
		Named(const Engine::Named&) = delete;
		void operator=(const Engine::Named&) = delete;
		~Named();

		const String& getName() const;
		void updateName(const String& newName);
	};
}
