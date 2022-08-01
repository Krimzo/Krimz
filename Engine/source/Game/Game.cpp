#include "Game/Game.h"


Krimz::Game::Game(const kl::uint2& windowSize, const std::string& gameName) : m_Window(windowSize, gameName) {
#ifndef _DEBUG
	kl::console::hide();
#endif
}

Krimz::Game::~Game() {

}

void Krimz::Game::start() {
	m_Window.maximize();

	m_Renderer.initalize(m_Window);

	bind(kl::make<Scene>());

	m_Timer.newInterval();
	m_Timer.newElapsed();
}

void Krimz::Game::update() {
	m_Timer.newInterval();

	m_Physics.update(m_Timer.interval());

	m_Renderer.clear();

	m_Renderer.render();

	m_Renderer.swap();
}

void Krimz::Game::end() {

}

void Krimz::Game::bind(kl::ref<Scene> scene) {
	m_Scene = scene;
	m_Renderer.bind(scene);
	m_Physics.bind(scene);
}

void Krimz::Game::run() {
	start();
	while (m_Window.process(false)) {
		update();
	}
	end();
}
