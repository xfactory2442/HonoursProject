#include "NPC.h"
#include <iostream>
#include <math.h>
#include "Grid.h"
#include "QuestManager.h"
#include "ResourceManager.h"

NPC::NPC(std::string name, int health, sf::Vector2f position, 
	sf::Texture* texture, CharacterManager* character_manager,
	int movements, int attacks, QuestManager* quest_manager) :
	Character(name, health, position, texture, character_manager,
		movements, attacks)
{
	quest_manager_ = quest_manager;
}

NPC::~NPC()
{
	for (int i = 0; i < 4; i++) {
		if (grid_node_->GetNeighbour(i)) {
			grid_node_->GetNeighbour(i)->SetLocation(nullptr);
		}
	}
}

void NPC::DoAction(float dt, Grid* grid)
{
	/*If the npc is not at max health, alterate turns between healing and not 
	healing the npc. If the NPC is at full health, set heal so that they heal 
	the next time they are not at full health on their turn.*/
	if (GetHealth() != max_health_) {
		heal = !heal;
		if (heal) {
			SubtractHealth(-1);
		}
	}
	else {
		heal = true;
	}
	//Generate quests then end the npc's turn.
	quest_manager_->GenerateQuests(this);
	TurnManager::DetermineCharacterTurn();
}

void NPC::IncrementNeeds()
{
	for (auto& need : Needs_) {
		need.second = std::min((int)need.second + 1, 9);
	}
}

void NPC::AddNeed()
{
	bool finished = false;
	while (!finished) {
		uint32_t random = rand() % ResourceManager::resources.size();
		std::unordered_map<std::string, uint32_t>::iterator it =
			ResourceManager::resources.begin();
		std::advance(it, random);
		//Insert a need into the needs map.
		if (Needs_.find((*it).first) == Needs_.end()) {
			Needs_.insert(std::pair<std::string, int>((*it).first, rand() % 10));
			finished = true;
		}
	}
}

std::unordered_map<std::string, uint32_t> NPC::GetAllNeeds()
{
	return Needs_;
}

void NPC::AddCompletedQuest(Quest* quest)
{
	completed_quests_.push_back(quest);
	character_manager_->ChangeRelationshipsAfterQuest(this);
	for (auto& quest_req : quest->GetRequirements()) {
		std::unordered_map<std::string, uint32_t>::iterator it =
			Needs_.find(quest_req.resource_);
		if (it != Needs_.end()) {
			(*it).second = 0;
		}
	}
}

std::vector<Quest*> NPC::GetCompletedQuests()
{
	return completed_quests_;
}

void NPC::AddRelationshipWithCharacter(std::string character_name, unsigned int rel)
{
	if (Relationship_.find(character_name) == Relationship_.end()) {
		Relationship_.insert(std::pair<std::string, unsigned int>(character_name, rel));
	}
}

void NPC::DeleteRelationshipWithCharacter(std::string character_name)
{
	std::unordered_map<std::string, uint32_t>::iterator it = Relationship_.find(character_name);
	if (it != Relationship_.end()) {
		Relationship_.erase(it);
	}
}

unsigned int NPC::GetRelationshipWithCharacter(std::string character_name)
{
	return Relationship_[character_name];
}

void NPC::ChangeRelationshipWithCharacter(std::string character_name, int change)
{
	if (Relationship_.find(character_name) != Relationship_.end()) {
		Relationship_[character_name] = 
			(uint32_t)fminf(fmaxf(float(Relationship_[character_name] + change), 0.0f), 9.0f);
		if (character_name == "Player") {
			relationship_change_ += change;
		}
	}
}

std::unordered_map<std::string, uint32_t> NPC::GetRelationships()
{
	return Relationship_;
}
