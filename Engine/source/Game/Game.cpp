#include "Game/Game.h"


Krimz::Game::Game(const kl::uint2& windowSize, const std::string& gameName)
{
	m_Window.start = std::bind(&Game::start, this);
	m_Window.update = std::bind(&Game::update, this);
	m_Window.end = std::bind(&Game::end, this);
	m_Window.run(windowSize, gameName, true, true);
}

Krimz::Game::~Game()
{

}

void Krimz::Game::start()
{
	m_Window.maximize();

	m_Renderer.initalize(m_Window);

	m_Timer.newInterval();
	m_Timer.newElapsed();
}

void Krimz::Game::update()
{
	m_Timer.newInterval();

	//m_Physics.update(m_Timer.interval());

	m_Renderer.clear();

	m_Renderer.render();

	m_Renderer.swap();
}

void Krimz::Game::end()
{

}

void Krimz::Game::bind(kl::ref<Scene> scene)
{
	m_Scene = scene;
}
