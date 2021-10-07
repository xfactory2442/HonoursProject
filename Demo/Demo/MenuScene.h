#pragma once
#include "Scene.h"
#include "Button.h"

class MenuScene : public Scene
{
public:
	MenuScene();
	bool Update(float dt);
	void Render();

private:
	Button* to_main_button_ = nullptr;
};

