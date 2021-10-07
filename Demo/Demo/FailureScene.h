#pragma once
#include "Scene.h"
#include "Button.h"

class FailureScene : public Scene
{
public:
	FailureScene();
	bool Update(float dt);
	void Render();
private:
	Button* to_menu_button_;
};

