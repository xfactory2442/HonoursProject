#include "SceneManager.h"

void SceneManager::ChangeScene(Scene scene)
{
	scene_ = scene;
	scene_changed_ = true;
}

void SceneManager::ChangeScene(int index)
{
	scene_ = (Scene)index;
	scene_changed_ = true;
}

SceneManager::Scene SceneManager::GetScene()
{
	return scene_;
}

void SceneManager::SetLocation(Location* location)
{
	current_location_ = location;
	scene_changed_ = true;
}

Location* SceneManager::GetLocation()
{
	return current_location_;
}
