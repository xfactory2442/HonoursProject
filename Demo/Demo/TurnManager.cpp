#include "TurnManager.h"
#include "Grid.h"
#include "CharacterManager.h"
#include "QuestManager.h"

void TurnManager::StartTurn(Character* character)
{
	//If the characters moveable was set to false then retain that for the end of the turn.
	was_moveable_ = character->GetMoveable();
	character->SetMoveable(true);
	Grid::grid_view_.setCenter(character->getPosition());
}

void TurnManager::EndTurn(Character* character)
{
	//Return the bool for the characters moveable area to what it was when they started their turn.
	character->SetMoveable(was_moveable_);
}

void TurnManager::DetermineCharacterTurn()
{
	//End the previous characters turn.
	if (character_turn_) {
		EndTurn(character_turn_);
		character_turn_ = nullptr;
	}
	character_turn_num_++;
	switch (turn_) {
	case Turn::Player:
		/*If the player just finished their turn, check if there are any NPCs. If there
		is then set the first NPC to have their turn. If there is no NPCs, set turn
		to enemys turn.*/
		if (character_manager_->Npcs_.size() > 0) {
			turn_ = Turn::NPC;
			character_turn_ = character_manager_->Npcs_[0];
			character_turn_num_ = 0;
			break;
		}
		turn_ = Turn::Enemy;
		character_turn_num_ = 0;
		//If there are any enemys, set it to their turn.
		if (character_manager_->Enemies_.size() > 0) {
			character_turn_ = character_manager_->Enemies_[0];
			break;
		}
		return;
	case Turn::NPC:
		/*If all of the NPCs have had their turn set the enemies to have their turn.*/
		if (character_turn_num_ >= character_manager_->Npcs_.size()) {
			character_turn_num_ = 0;
			turn_ = Turn::Enemy;
			if (character_manager_->Enemies_.size() > 0) {
				character_turn_ = character_manager_->Enemies_[0];
			}
		}
		else {
			character_turn_ = character_manager_->Npcs_[character_turn_num_];
		}
		break;
	case Turn::Enemy:
		/*If all of the enemies have had their turn spawn more enemies
		then set the player to have their turn, else set the next enemy to have 
		their turn.*/
		if (character_turn_num_ >= character_manager_->Enemies_.size()) {
			character_manager_->SpawnEnemies();
			turn_ = Turn::Player;
			turn_num_++;
			character_turn_ = character_manager_->player_;
		}
		else {
			character_turn_ = character_manager_->Enemies_[character_turn_num_];
		}
	}
	//Start the determined characters turn.
	if (character_turn_) {
		turn_timer_ = max_turn_timer_;
		StartTurn(character_turn_);
	}
}

void TurnManager::DEBUGSkipToStartOfPlayerTurn()
{
	EndTurn(character_turn_);
	StartTurn(character_turn_);
}

bool TurnManager::TurnTimer(float dt)
{
	turn_timer_ -= dt;
	if (turn_timer_ <= 0.0f) {
		return true;
	}
	return false;
}
