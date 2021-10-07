#pragma once
#include "Character.h"
#include "NPC.h"

class Enemy : public Character
{
public:
	Enemy(int health, sf::Vector2f position, sf::Texture* texture, 
		CharacterManager* character_manager, int movements, int attacks, 
		std::string name, std::string drop, int movement, int attack_strength);
	~Enemy();

	void DoAction(float dt, Grid* grid);
	std::string GetDropName();

private:
	enum class TurnStages {
		FindTarget,
		Movement,
		Attack,
		Count
	};

	void IncrementTurn();

	Character* target_ = nullptr;
	TurnStages stage_ = TurnStages::FindTarget;

	std::string drop_;

	float timer_ = 0.0f;
	const float time_between_actions_ = 0.3f;
};

