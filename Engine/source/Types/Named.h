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
		std::string m_Name;
		Type m_Type;

	public:
		Named(Named::Type type);
		Named(Named::Type type, const std::string& name);
		Named(const Engine::Named&) = delete;
		void operator=(const Engine::Named&) = delete;
		~Named();

		const std::string& getName() const;
		void updateName(const std::string& newName);
	};
}
