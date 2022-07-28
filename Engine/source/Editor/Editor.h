#pragma once

#include "KrimzLib.h"
#include "Logger/Logger.h"
#include "Game/Game.h"
#include "GUI/GUIRenderer.h"


namespace Krimz
{
	class Editor
	{
		kl::window m_Window = {};
		kl::timer m_Timer = {};
		Logger m_Logger = {};
		Physics m_Physics = {};
		Renderer m_Renderer = {};
		GUIRenderer m_GUIRenderer = {};

		kl::ref<Scene> m_Scene;
		kl::ref<Game> m_Game;

		void start();
		void update();
		void end();

	public:
		Editor(const kl::uint2 windowSize);
		Editor(const Editor&) = delete;
		void operator=(const Editor&) = delete;
		~Editor();

		void bind(kl::ref<Game> game);
	};
}
