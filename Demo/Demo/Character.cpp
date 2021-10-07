#include "Character.h"
#include "CharacterManager.h"
#include "Grid.h"

Character::Character(std::string name, int health, sf::Vector2f position,
	sf::Texture* texture, CharacterManager* character_manager, 
	unsigned int num_movement, unsigned int num_attacks, unsigned int attack_strength) :
	GridObject(sf::Vector2f(Grid::grid_spacing_ / 5.0f * 4.0f,
		Grid::grid_spacing_ / 5.0f * 4.0f) , position, texture)
{
	//Setup the health text.
	health_text_.setCharacterSize(30);
	health_text_.setFillColor(sf::Color::White);
	health_text_.setFont(GeneralVariables::font_);
	health_text_.setOutlineColor(sf::Color::Black);
	health_text_.setOutlineThickness(2.0f);

	max_health_ = health;
	//Set health to be the max health.
	ResetHealth();
	name_ = name;
	//Characters cannot stand on top of other characters.
	overlap_ = false;
	character_manager_ = character_manager;
	max_movement_actions_ = num_movement;
	max_actions_ = num_attacks;
	attack_strength_ = attack_strength;

	//Initialise the icons which show what stats the character has.
	movement_icon_ = new Icon(sf::Vector2f(5.0f, 5.0f), sf::Vector2f(40.0f, 50.0f));
	movement_icon_->setFillColor(sf::Color::Blue);
	actions_icon_ = new Icon(sf::Vector2f(55.0f, 5.0f), sf::Vector2f(40.0f, 50.0f));
	actions_icon_->setFillColor(sf::Color::Red);
	attack_strength_icon_ = new Icon(sf::Vector2f(105.0f, 5.0f), sf::Vector2f(40.0f, 50.0f));
	attack_strength_icon_->setFillColor(sf::Color(200, 0, 0, 255));
	attack_strength_icon_->SetText(std::to_string(attack_strength_));

	setOrigin(getSize() / 2.0f);

	//Set movement and normal actions to their maximums.
	ResetActions();
}

Character::~Character()
{
	grid_node_->SetCharacterOnTile(nullptr);
}

void Character::ResetHealth()
{
	health_ = max_health_;
	SetHealthText();
}

bool Character::SubtractHealth(int health)
{
	//Remove health from a character then check if that character is dead.
	health_ -= health;
	health_ = (int)fminf((float)health_, (float)max_health_);
	SetHealthText();
	if (health_ <= 0) {
		return true;
	}
	return false;
}

int Character::GetHealth()
{
	return health_;
}

void Character::AddMemory(std::vector<std::string> what_happened)
{
	Memories::AddMemory(what_happened, grid_node_);
}

uint32_t Character::GetMovement()
{
	return movement_;
}

uint32_t Character::GetMaxMovementActions()
{
	return max_movement_actions_;
}

uint32_t Character::GetMovementActions()
{
	return num_movement_actions_;
}

uint32_t Character::GetMaxActions()
{
	return max_actions_;
}

uint32_t Character::GetActions()
{
	return num_actions_;
}

bool Character::GetMoveable()
{
	return show_movement_;
}

void Character::SetMoveable(bool moveable)
{
	show_movement_ = moveable;
}

void Character::InvertMoveable()
{
	show_movement_ = !show_movement_;
}

bool Character::SpendMovement()
{
	//If there is a movement point, allow the character to move.
	if (num_movement_actions_ > 0) {
		num_movement_actions_--;
		movement_icon_->SetText(std::to_string(num_movement_actions_));
		return true;
	}
	return false;
}

bool Character::SpendAction()
{
	//If there is an action point, allow the character to complete their action.
	if (num_actions_ > 0) {
		num_actions_--;
		actions_icon_->SetText(std::to_string(num_actions_));
		return true;
	}
	return false;
}

void Character::RenderMoveableArea()
{
	if (show_movement_) {
		for (Node* node : Moveable_Nodes_) {
			node->RenderMoveable();
		}
	}
}

void Character::RenderIcons()
{
	actions_icon_->Render();
	movement_icon_->Render();
	attack_strength_icon_->Render();
}

void Character::RenderHealthText()
{
	GeneralVariables::window_.draw(health_text_);
}

bool Character::isEnemy()
{
	return is_enemy_;
}

std::string Character::GetName()
{
	return name_;
}

void Character::ChangeMaxHealth(int diff)
{
	max_health_ += diff;
	if (health_ > max_health_) {
		health_ = max_health_;
		SetHealthText();
	}
}

void Character::ChangeMaxMovementActions(int diff)
{
	max_movement_actions_ += diff;
}

void Character::ChangeMaxActions(int diff)
{
	max_actions_ += diff;
}

void Character::ChangeMovement(int diff)
{
	movement_ += diff;
}

void Character::ChangeAttackStrength(int diff)
{
	attack_strength_ += diff;
	attack_strength_icon_->SetText(std::to_string(attack_strength_));
}

int Character::GetMaxHealth()
{
	return max_health_;
}

uint32_t Character::GetAttackStrength()
{
	return attack_strength_;
}

float Character::GetDistanceFromCharacter(Character* character)
{
	//Get the distance between this character and another.
	sf::Vector2i position1 = GetGridNode()->GetGridPosition();
	sf::Vector2i position2 = character->GetGridNode()->GetGridPosition();
	return sqrtf(powf((float)position1.x - (float)position2.x, 2.0f) + 
		powf((float)position1.y - (float)position2.y, 2.0f));
}

void Character::MoveObject(sf::Vector2f position)
{
	//Move all parts of the character as one.
	setPosition(position);
	health_text_.setPosition(position - sf::Vector2f(9.0f, 20.0f));
}

void Character::ResetActions()
{
	num_movement_actions_ = max_movement_actions_;
	num_actions_ = max_actions_;
	actions_icon_->SetText(std::to_string(num_actions_));
	movement_icon_->SetText(std::to_string(num_movement_actions_));
}

void Character::SetHealthText()
{
	health_text_.setString(std::to_string(health_));
}
