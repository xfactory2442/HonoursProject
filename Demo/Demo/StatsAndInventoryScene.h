#pragma once
#include "Scene.h"

class StatsAndInventoryScene : public Scene
{
public:
	StatsAndInventoryScene();
	bool Update(float dt);
	void Render();

	void OnEnter();

private:
	sf::Text text_;
};

