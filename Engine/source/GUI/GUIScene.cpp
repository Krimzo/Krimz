#include "GUI/GUI.h"


void Engine::GUI::SceneRender() {
	Scene::Cameras();
	Scene::Entites();
	Scene::Meshes();
	Scene::Skyboxes();
	Scene::Textures();
}
