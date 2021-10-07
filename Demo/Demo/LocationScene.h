#pragma once
#include "Scene.h"
#include "Location.h"
#include "CharacterManager.h"

class LocationScene : public Scene
{
public:
	LocationScene(CharacterManager* character_manager_, InfoWindow* info_window);
	void OnEnterLocation();
	bool Update(float dt);
	void Render();

	static float failed_action_timer_;

private:
	InfoWindow* info_window_ = nullptr;
	Location* location_ = nullptr;
	CharacterManager* character_manager_;
	sf::Text failed_action_text_;
};

