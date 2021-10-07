#include "Enemy.h"
#include "Grid.h"
#include "CharacterManager.h"
#include <iostream>

Enemy::Enemy(int health, sf::Vector2f position, sf::Texture* texture,
	CharacterManager* character_manager, int movements, int attacks, std::string name,
	std::string drop, int movement, int attack_strength) :
	Character(name, health, position, texture, character_manager, movements, attacks)
{
	is_enemy_ = true;
	drop_ = drop;
	movement_ = movement;
	attack_strength_ = attack_strength;
}

Enemy::~Enemy()
{
	target_ = nullptr;
}

void Enemy::DoAction(float dt, Grid* grid)
{
	if (timer_ > 0.0f) {
		timer_ -= dt;
		return;
	}
	switch (stage_)
	{
	case TurnStages::FindTarget:
		//Find the closest target.
		target_ = grid->FindClosestTarget(this);
		IncrementTurn();
		break;
	case TurnStages::Movement:
		//Move towards the target.
		if (GetMovementActions() > 0 && target_) {
			grid->MoveCharacterTowardsTarget(this, target_);
			SpendMovement();
		}
		if (GetMovementActions() <= 0) {
			IncrementTurn();
		}
		else {
			//If there is more movement left then evaluate target a second time.
			stage_ = TurnStages::FindTarget;
		}
		timer_ = time_between_actions_;
		break;
	case TurnStages::Attack:
		//Attack the target.
		if (target_ && GetActions() > 0 &&
			grid->CheckIfInRange(GetGridNode(), target_->GetGridNode(), 1)) {
			if (target_->GetName() == "Player") {
				Player* player = (Player*)target_;
				player->SetInfoWindow(false, attack_strength_);
			}
			else if (target_->SubtractHealth(attack_strength_)) {
				character_manager_->DeleteDeadGoodCharacter(target_->GetName());
				target_ = nullptr;
			}
			SpendAction();
		}
		else {
			IncrementTurn();
			TurnManager::DetermineCharacterTurn();
			ResetActions();
		}
		timer_ = time_between_actions_;
		break;
	}
}

std::string Enemy::GetDropName()
{
	return drop_;
}

void Enemy::IncrementTurn()
{
	stage_ = TurnStages((int)stage_ + 1 == (int)TurnStages::Count ?
		0 : (int)stage_ + 1);
}

