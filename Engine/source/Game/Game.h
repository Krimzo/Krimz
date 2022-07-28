#pragma once

#include "KrimzLib.h"
#include "Physics/Physics.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"


namespace Krimz
{
	class Game
	{
	protected:
		kl::window m_Window = {};
		kl::timer m_Timer = {};
		Physics m_Physics = {};
		Renderer m_Renderer = {};

		kl::ref<Scene> m_Scene;

		void start();
		void update();
		void end();

	public:
		Game(const kl::uint2& windowSize, const std::string& gameName);
		Game(const Game&) = delete;
		void operator=(const Game&) = delete;
		~Game();

		void bind(kl::ref<Scene> scene);
	};
}
