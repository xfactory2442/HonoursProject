#include "Scene.h"

void Scene::StartDraw()
{
	//Clear the window to black.
	GeneralVariables::window_.clear(sf::Color::Black);
}

void Scene::EndDraw()
{
	//Display the frame in the window.
	GeneralVariables::window_.display();
}
