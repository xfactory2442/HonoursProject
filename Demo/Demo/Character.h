#pragma once
#include "GridObject.h"
#include "Memories.h"
#include "TurnManager.h"
#include "Icon.h"

class Grid;
class CharacterManager;

class Character : public GridObject, public Memories
{
public:
	Character(std::string name, int health, sf::Vector2f position,
		sf::Texture* texture, CharacterManager* character_manager, 
		unsigned int num_movement, unsigned int num_attacks, 
		unsigned int attack_strength = 1);
	~Character();

	virtual void DoAction(float dt, Grid* grid) = 0;

	//Reset the health of the character to max health.
	void ResetHealth();
	//Remove health from character and delete them if they have no health left.
	bool SubtractHealth(int health);
	//Return the amount of health remaining.
	int GetHealth();

	void AddMemory(std::vector<std::string> what_happened);
	uint32_t GetMovement();
	bool GetMoveable();
	void SetMoveable(bool moveable);
	void InvertMoveable();

	uint32_t GetMaxMovementActions();
	uint32_t GetMovementActions();
	uint32_t GetMaxActions();
	uint32_t GetActions();
	bool SpendMovement();
	bool SpendAction();

	void RenderMoveableArea();
	void RenderIcons();
	void RenderHealthText();

	std::vector<Node*> Moveable_Nodes_;

	bool isEnemy();

	std::string GetName();

	void ChangeMaxHealth(int diff);
	void ChangeMaxMovementActions(int diff);
	void ChangeMaxActions(int diff);
	void ChangeMovement(int diff);
	void ChangeAttackStrength(int diff);
	int GetMaxHealth();
	uint32_t GetAttackStrength();

	float GetDistanceFromCharacter(Character* character);
	void MoveObject(sf::Vector2f position);

protected:
	void ResetActions();

	bool is_enemy_ = false;

	uint32_t attack_strength_ = 1;

	std::string name_ = "";
	uint32_t movement_ = 4;
	bool show_movement_ = false;

	CharacterManager* character_manager_ = nullptr;

	int max_health_ = 2;

private:
	void SetHealthText();

	int health_ = 0;
	sf::Text health_text_;

	uint32_t num_movement_actions_ = 0;
	uint32_t max_movement_actions_ = 1;
	Icon* movement_icon_ = nullptr;
	uint32_t num_actions_ = 0;
	uint32_t max_actions_ = 1;
	Icon* actions_icon_ = nullptr;
	Icon* attack_strength_icon_ = nullptr;
};