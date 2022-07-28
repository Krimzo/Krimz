#include "GUI/GUI.h"


void Krimz::GUI::SceneRender()
{
	Scene::Cameras();
	Scene::Entites();
	Scene::Meshes();
	Scene::Skyboxes();
	Scene::Textures();
}
