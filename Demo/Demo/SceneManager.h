#pragma once
#include "Location.h"

class SceneManager
{
public:
	enum class Scene {
		Menu,
		Main,
		Location,
		Failure,
		StatsAndInventory
	};

	static void ChangeScene(Scene scene_);
	static void ChangeScene(int index);
	static Scene GetScene();

	static void SetLocation(Location* location);
	static Location* GetLocation();

	static bool scene_changed_;

private:
	static Scene scene_;
	static Location* current_location_;
};

