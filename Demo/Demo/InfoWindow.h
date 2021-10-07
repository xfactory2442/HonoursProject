#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GeneralFunctions.h"

using namespace std;

class Player;

class InfoWindow : public sf::RectangleShape
{
public:
	InfoWindow();
	void InitialisePlayer(Player* player);
	void Collsion();
	void Render();

	bool isAlive();
	void SetAlive(bool a);
	void ShowWindow(bool reward, int num);
	void SetStats();

private:
	struct StatChanges {
		bool alive = true;
		sf::Text stat_text_;
		Button* button_ = nullptr;
	};
	void InitialiseStatChanges();
	StatChanges InitialiseStatChangesDetails(float positionY, StatChanges stat);
	void HandleStatChange(sf::Vector2f mouse_position);
	void IncreaseNumberOfStatChanges(int num);

	bool alive_ = false;
	bool reward_ = false;
	Player* player_ = nullptr;
	std::vector<StatChanges> Stat_Changes_;
	sf::Text health_text_;
	Button* skip_button_ = nullptr;

	sf::Vector2f window_size_ = sf::Vector2f(500.0f, 410.0f);
	sf::Vector2f button_size_ = sf::Vector2f(80.0f, 40.0f);

	sf::Text num_changes_text_;
	int num_changes_ = 0;
};

