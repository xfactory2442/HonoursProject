#pragma once
#include "TaskLocation.h"
#include "Task.h"
#include "CharacterManager.h"

class QuestLocation;

class Grid;

class LocationManager
{
public:
	~LocationManager();
	void DeleteAllQuestLocations();
	void CreateQuestLocation(Grid* grid, NPC* npc,
		QuestManager* quest_manager);
	void CreateTaskLocations(Grid* grid);
	void AddTaskToLocations(std::string task_name, std::string resource_name,
		std::vector<std::string> resource_names, std::vector<int> resource_amounts);
	void DeleteQuestLocation(QuestLocation* quest_location);
	void DeleteAllTaskLocations();

private:
	std::vector<TaskLocation*> Task_Locations_;
	std::vector<QuestLocation*> Quest_Locations_;
	sf::Texture task_location_texture_;
};

