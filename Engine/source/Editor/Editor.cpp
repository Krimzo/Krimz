#include "Editor/Editor.h"


Krimz::Editor::Editor(const kl::uint2 windowSize)
{
	m_Window.start = std::bind(&Editor::start, this);
	m_Window.update = std::bind(&Editor::update, this);
	m_Window.end = std::bind(&Editor::end, this);
	m_Window.run(windowSize, "Krimz Editor", true, true);
}

Krimz::Editor::~Editor()
{

}

void Krimz::Editor::start()
{
	m_Window.icon("resource/textures/icons/k.ico");
	m_Window.maximize();

	m_Renderer.initalize(m_Window);

	m_GUIRenderer.initialize(m_Window);
	m_GUIRenderer.initialize(m_Renderer.gpu());

	m_Timer.newInterval();
	m_Timer.newElapsed();
}

void Krimz::Editor::update()
{
	m_Timer.newInterval();

	//m_Physics.update(m_Timer.interval());

	m_Renderer.clear();

	m_Renderer.render();

	m_GUIRenderer.render();

	m_Renderer.swap();
}

void Krimz::Editor::end()
{

}

void Krimz::Editor::bind(kl::ref<Game> game)
{
	m_Game = game;
}
