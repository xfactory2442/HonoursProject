#include "CharacterManager.h"
#include "SceneManager.h"
#include <sstream>
#include <fstream>
#include "Grid.h"
#include <iostream>


void CharacterManager::DeleteAll()
{
	//Delete all the characters.
	for (int i = 0; i < Enemies_.size(); i++) {
		delete Enemies_.back();
		Enemies_.pop_back();
	}

	for (int i = 0; i < Npcs_.size(); i++) {
		delete Npcs_.back();
		Npcs_.pop_back();
	}

	delete player_;
	player_ = NULL;
}

void CharacterManager::DeleteDeadGoodCharacter(std::string character_name)
{
	//Delete all characters that arent enemies by name.
	//If the player dies then the game is over so swap to the failure scene.
	if (character_name == player_->GetName()) {
		SceneManager::ChangeScene(SceneManager::Scene::Failure);
		return;
	}
	for (int i = 0; i < Npcs_.size(); i++) {
		if (character_name == Npcs_[i]->GetName()) {
			//Delete the npcs quest locations.
			for (int i = 0; i < 4; i++) {
				Node* neighbour = Npcs_[i]->GetGridNode()->GetNeighbour(i);
				if (neighbour) {
					neighbour->SetLocation(nullptr);
					location_manager_->DeleteQuestLocation((QuestLocation*)neighbour->GetLocation());
					break;
				}
			}
			//Remove the npc from the node it's stood on.
			Npcs_[i]->GetGridNode()->SetCharacterOnTile(nullptr);
			//Remove the relationships of other npcs that relate to this npc.
			DeleteDeadNPCsRelationships(Npcs_[i]->GetName());
			delete Npcs_[i];
			Npcs_.erase(Npcs_.begin() + i);
			return;
		}
	}
}

void CharacterManager::DeleteDeadEnemy(Enemy* enemy)
{
	//Delete enemies.
	for (int i = 0; i < Enemies_.size(); i++) {
		if (enemy == Enemies_[i]) {
			delete Enemies_[i];
			Enemies_.erase(Enemies_.begin() + i);
			return;
		}
	}
}

void CharacterManager::SpawnEnemies()
{
	if (Enemies_.size() < max_enemies_) {
		/*Get the spots on the map that do not have a character already in them from
		the spawn point pool.*/
		std::vector<Node*> current_open_spots = Enemy_Spawn_Points_;
		for (int i = 0; i < current_open_spots.size(); i++) {
			if (current_open_spots[i]->GetCharacterOnTile()) {
				current_open_spots.erase(current_open_spots.begin() + i);
				i--;
			}
		}

		//Get a random number of characters that will spawn.
		int random = rand() % num_to_spawn_;
		//If there was no enemies spawned last enemy turn then definitely spawn at 
		//least one enemy.
		if (!spawned_before_) {
			random = random == 0 ? 1 : random;
			spawned_before_ = true;
		}
		else if (random == 0) {
			spawned_before_ = false;
		}
		//For the amount of enemies decided, if there is a spot open, spawn an enemy
		//based on the types loaded in from file.
		for (int i = 0; i < random; i++) {
			if (current_open_spots.size() > 0) {
				Enemies_.push_back(ChooseEnemyType());
				Enemies_.back()->setFillColor(sf::Color::Red);
				int random2 = rand() % current_open_spots.size();
				grid_->InitialiseCharacter(Enemies_.back(), current_open_spots[random2]);
				current_open_spots.erase(current_open_spots.begin() + random2);
			}
			else {
				break;
			}
		}
	}
}

void CharacterManager::SetLocationManager(LocationManager* location_manager)
{
	location_manager_ = location_manager;
}

void CharacterManager::SetGrid(Grid* grid)
{
	grid_ = grid;
}

void CharacterManager::SetQuestManager(QuestManager* quest_manager)
{
	quest_manager_ = quest_manager;
}

void CharacterManager::AddEnemySpawnPoint(Node* node)
{
	if (std::find(Enemy_Spawn_Points_.begin(), Enemy_Spawn_Points_.end(), node) == Enemy_Spawn_Points_.end()) {
		Enemy_Spawn_Points_.push_back(node);
	}
}

std::string CharacterManager::GetEnemyDropByEnemyName(std::string enemy_name)
{
	for (auto enemy : Enemy_Types_) {
		if (enemy.name_ == enemy_name) {
			return enemy.drop_;
		}
	}
	return std::string();
}

void CharacterManager::CreateRelationship(NPC* npc, std::string other_character)
{
	//If the other character is a player then only create the relationship on one side.
	if (other_character == "Player") {
		int num = rand() % 10;
		npc->AddRelationshipWithCharacter("Player", num);
	}
	//If both characters are npcs then have both npcs create relationships to each other.
	else {
		for (int i = 0; i < Npcs_.size(); i++) {
			if (Npcs_[i]->GetName() == other_character) {
				int random = rand() % 10;
				npc->AddRelationshipWithCharacter(other_character, random);
				//Have the second npc have a similar relationship to the first.
				random += std::max(std::min((rand() % 5 - 2), 9), 0);
				Npcs_[i]->AddRelationshipWithCharacter(npc->GetName(), random);
				return;
			}
		}
	}
}

void CharacterManager::DeleteDeadNPCsRelationships(std::string npcs_name)
{
	for (int i = 0; i < Npcs_.size(); i++) {
		Npcs_[i]->DeleteRelationshipWithCharacter(npcs_name);
	}
}

void CharacterManager::ChangeRelationshipsAfterQuest(NPC* npc)
{
	/*Depending on the relationships between npcs, lower or increase the
	relationships between them.*/
	npc->ChangeRelationshipWithCharacter("Player", 1);
	for (auto& relationship : npc->GetRelationships()) {
		NPC* other_npc = FindNPCByName(relationship.first);
		if (other_npc) {
			if (relationship.second > 6) {
				uint32_t weight = 9 - relationship.second;
				uint32_t random = 0;
				if (weight != 0) {
					random = rand() % weight;
				}
				if (random == 0) {
					other_npc->ChangeRelationshipWithCharacter("Player", 1);
				}
			}
			else if (relationship.second < 3) {
				uint32_t random = 0;
				if (relationship.second != 0) {
					random = rand() % relationship.second;
				}
				if (random == 0) {
					other_npc->ChangeRelationshipWithCharacter("Player", -1);
				}
			}
		}
	}
}

Enemy* CharacterManager::ChooseEnemyType()
{
	int random = rand() % Enemy_Types_.size();
	return new Enemy(Enemy_Types_[random].max_health_, sf::Vector2f(), nullptr,
		this, Enemy_Types_[random].movement_actions_, Enemy_Types_[random].attack_,
		Enemy_Types_[random].name_, Enemy_Types_[random].drop_, 
		Enemy_Types_[random].movement_, Enemy_Types_[random].attack_strength_);
}

void CharacterManager::LoadNPCNamesFromFile()
{
	std::fstream file;
	file.open("OtherFiles/NPCData.csv");
	std::string line, word, temp;

	while (file >> temp) {
		std::vector<std::string> row;

		std::getline(file, line);
		temp += line;

		std::stringstream s(temp);

		while (std::getline(s, word, ',')) {
			row.push_back(word);
		}

		if (row.size() > 0) {
			NPC_Details_.push_back(NPCDetails());
			NPC_Details_.back().name = row[0];
			sf::Texture* texture = nullptr;
			if (row.size() > 1 && row[1] != "") {
				texture->loadFromFile(row[1]);
			}
			NPC_Details_.back().texture = texture;
		}
	}
}

void CharacterManager::CreateNPC()
{
	int random = rand() % NPC_Details_.size();
	NPCDetails details = NPC_Details_[random];
	NPC_Details_.erase(NPC_Details_.begin() + random);
	Npcs_.push_back(new NPC(details.name, rand() % 6 + 5, sf::Vector2f(), 
		nullptr, this, 1, 0, quest_manager_));
	Npcs_.back()->setFillColor(sf::Color::Blue);
	random = rand() % 3 + 2;
	for (int i = 0; i < random; i++) {
		Npcs_.back()->AddNeed();
	}
	sf::Vector2i initial_position = sf::Vector2i(0, 0);
	do {
		initial_position = sf::Vector2i(rand() % 10 - 5, rand() % 10 - 5);
	} while (!grid_->InitialiseCharacter(Npcs_.back(), initial_position));

	//Create relationships between all characters.
	CreateRelationship(Npcs_.back(), "Player");
	for (int i = 0; i < Npcs_.size() - 1; i++) {
		CreateRelationship(Npcs_.back(), Npcs_[i]->GetName());
	}
	location_manager_->CreateQuestLocation(grid_, Npcs_.back(), quest_manager_);
}

NPC* CharacterManager::FindNPCByName(std::string name)
{
	for (auto& npc : Npcs_) {
		if (npc->GetName() == name) {
			return npc;
		}
	}
	return nullptr;
}

void CharacterManager::LoadEnemyTypesFromFile(std::string file_name)
{
	//Read in the enemy's data from file.
	EnemyType enemy;

	std::fstream file;
	file.open(file_name);
	std::string line, word, temp;

	while (file >> temp) {
		std::vector<std::string> row;

		std::getline(file, line);
		temp += line;

		std::stringstream s(temp);

		while (std::getline(s, word, ',')) {
			row.push_back(word);
		}

		if (row.size() > 0) {
			if (row[0] == "name") {
				enemy.name_ = row[1];
			}
			else if (row[0] == "drop") {
				enemy.drop_ = row[1];
			}
			else if (row[0] == "max health") {
				enemy.max_health_ = std::stoi(row[1]);
			}
			else if (row[0] == "texture location") {
				enemy.texture_location_ = row[1];
			}
			else if (row[0] == "movement") {
				enemy.movement_ = std::stoi(row[1]);
			}
			else if (row[0] == "movement actions") {
				enemy.movement_actions_ = std::stoi(row[1]);
			}
			else if (row[0] == "attack") {
				enemy.attack_ = std::stoi(row[1]);
			}
			else if (row[0] == "attack strength") {
				enemy.attack_strength_ = std::stoi(row[1]);
			}
		}
	}
	Enemy_Types_.push_back(enemy);
}

void CharacterManager::RenderAll()
{
	for (int i = 0; i < Npcs_.size(); i++) {
		Npcs_[i]->RenderMoveableArea();
	}

	for (int i = 0; i < Enemies_.size(); i++) {
		Enemies_[i]->RenderMoveableArea();
	}

	if (player_) {
		player_->RenderMoveableArea();
	}

	//Render every npc.
	for (int i = 0; i < Npcs_.size(); i++) {
		if (Npcs_[i]) {
			GeneralVariables::window_.draw(*Npcs_[i]);
			Npcs_[i]->RenderHealthText();
		}
	}

	//Render every enemy.
	for (int i = 0; i < Enemies_.size(); i++) {
		if (Enemies_[i]) {
			GeneralVariables::window_.draw(*Enemies_[i]);
			Enemies_[i]->RenderHealthText();
		}
	}

	//Render the player
	if (player_) {
		GeneralVariables::window_.draw(*player_);
		player_->RenderHealthText();
	}
}
