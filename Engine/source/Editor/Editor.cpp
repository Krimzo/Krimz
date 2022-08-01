#include "Editor/Editor.h"
#include "GUI/Sections/GUISections.h"


Krimz::Editor::Editor(const kl::uint2 windowSize) : m_Window(windowSize, "Krimz Editor") {
#ifdef _DEBUG
	m_Window.title("Krimz Editor [Debug]");
	kl::console::title("Krimz Editor Console [Debug]");
#else
	kl::console::hide();
#endif
}

Krimz::Editor::~Editor() {

}

void Krimz::Editor::start() {
	m_Window.icon("resource/textures/icons/k.ico");
	m_Window.maximize();

	m_Renderer.initalize(m_Window);
	m_Window.resize = [&](const kl::uint2& newSize) {
		m_Renderer.resize(newSize);
	};

	m_GUIRenderer.initialize(m_Window);
	m_GUIRenderer.initialize(m_Renderer.gpu());

	LoadGuiSections(m_GUIRenderer);

	bind(kl::make<Scene>());

	m_Timer.newInterval();
	m_Timer.newElapsed();
}

void Krimz::Editor::update() {
	m_Timer.newInterval();

	m_Physics.update(m_Timer.interval());

	m_Renderer.clear();

	m_Renderer.render();

	m_GUIRenderer.render();

	m_Renderer.swap();
}

void Krimz::Editor::end() {

}

void Krimz::Editor::bind(kl::ref<Scene> scene) {
	m_Scene = scene;
	m_Renderer.bind(scene);
	m_Physics.bind(scene);
}

void Krimz::Editor::run() {
	start();
	while (m_Window.process()) {
		update();
	}
	end();
}
