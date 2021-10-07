#pragma once
#include <vector>
#include "Player.h"
#include "NPC.h"
#include "Enemy.h"
#include "LocationManager.h"

class CharacterManager
{
public:
	//The player character.
	Player* player_ = nullptr;
	//A vector of npcs that are alive.
	std::vector<NPC*> Npcs_;
	//Load in NPCs from file.
	void LoadNPCNamesFromFile();
	void CreateNPC();
	NPC* FindNPCByName(std::string name);
	//A vector of enemies that are alive.
	std::vector<Enemy*> Enemies_;
	const uint32_t max_enemies_ = 4;
	void LoadEnemyTypesFromFile(std::string file_name);
	
	void RenderAll();
	void DeleteAll();
	void DeleteDeadGoodCharacter(std::string character_name);
	void DeleteDeadEnemy(Enemy* enemy);
	void SpawnEnemies();

	void SetLocationManager(LocationManager* location_manager);
	void SetGrid(Grid* grid);
	void SetQuestManager(QuestManager* quest_manager);
	void AddEnemySpawnPoint(Node* node);

	std::string GetEnemyDropByEnemyName(std::string enemy_name);

	void CreateRelationship(NPC* npc, std::string other_character);
	void DeleteDeadNPCsRelationships(std::string npcs_name);
	void ChangeRelationshipsAfterQuest(NPC* npc);


private:
	Enemy* ChooseEnemyType();
	LocationManager* location_manager_ = nullptr;
	Grid* grid_ = nullptr;
	QuestManager* quest_manager_ = nullptr;
	
	//NPCVariables.
	struct NPCDetails {
		std::string name;
		sf::Texture* texture;
	};
	std::vector<NPCDetails> NPC_Details_;

	//Enemy variables.
	unsigned int num_to_spawn_ = 2;
	std::vector<Node*> Enemy_Spawn_Points_;
	bool spawned_before_ = false;

	struct EnemyType {
		std::string name_ = "";
		std::string drop_ = "";
		unsigned int max_health_ = 0;
		unsigned int movement_ = 0;
		unsigned int movement_actions_ = 0;
		unsigned int attack_ = 0;
		unsigned int attack_strength_ = 0;
		std::string texture_location_ = "";
	};
	std::vector<EnemyType> Enemy_Types_;
};

