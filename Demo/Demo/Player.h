#pragma once
#include "Character.h"
#include "Button.h"
#include "InfoWindow.h"

class Player : public Character
{
public:
	Player(int health, sf::Vector2f position, sf::Texture* texture, 
		CharacterManager* character_manager, unsigned int movements, 
		unsigned int attacks, InfoWindow* info_window);
	~Player();
	void DoAction(float dt, Grid* grid);
	void RenderTurnButton();

	void SetInfoWindow(bool reward, int num);
	void CheckDead();

private:
	void CheckIfSpaceEmptyAndResolve(Node* node, Grid* grid);
	void RelationshipChangeWhenEnemyDies();
	InfoWindow* info_window_ = nullptr;
	//Changes the turn from the players to the enemies.
	Button* turn_button_ = nullptr;
	Button* DEBUG_skip_to_start_of_players_turn_ = nullptr;

	//The character that has been clicked on.
	Character* selected_character_ = nullptr;

	
};