#include "LocationManager.h"
#include "Grid.h"
#include "NPC.h"
#include "QuestLocation.h"

LocationManager::~LocationManager()
{
	DeleteAllQuestLocations();
	DeleteAllTaskLocations();
}

void LocationManager::DeleteAllQuestLocations()
{
	while (Quest_Locations_.size() > 0) {

		delete Quest_Locations_.back();
		Quest_Locations_.pop_back();
	}
}

void LocationManager::CreateQuestLocation(Grid* grid, NPC* npc, 
	QuestManager* quest_manager)
{
	//Create quest locations for every npc and then make them accessible from all 
	//the nodes that neighbour the npc.
	Quest_Locations_.push_back(new QuestLocation(npc, npc->GetGridNode(), nullptr, quest_manager));
	for (int i = 0; i < 4; i++) {
		Node* neighbour = npc->GetGridNode()->GetNeighbour(i);
		if (neighbour) {
			neighbour->SetLocation(Quest_Locations_.back());
		}
	}
}

void LocationManager::CreateTaskLocations(Grid* grid)
{
	task_location_texture_.loadFromFile("gfx/exclamationMark.png");
	Task_Locations_.push_back(new TaskLocation(
		grid->GetNodeAtPositionOrClosest(sf::Vector2i(3, 3)),
		&task_location_texture_));
	Task_Locations_.push_back(new TaskLocation(
		grid->GetNodeAtPositionOrClosest(sf::Vector2i(-3, 3)),
		&task_location_texture_));
	Task_Locations_.push_back(new TaskLocation(
		grid->GetNodeAtPositionOrClosest(sf::Vector2i(-3, -3)),
		&task_location_texture_));
	Task_Locations_.push_back(new TaskLocation(
		grid->GetNodeAtPositionOrClosest(sf::Vector2i(3, -3)),
		&task_location_texture_));
}

void LocationManager::AddTaskToLocations(std::string task_name, 
	std::string resource_name, std::vector<std::string> resource_names, 
	std::vector<int> resource_amounts)
{
	int random = rand() % Task_Locations_.size();
	Task_Locations_[random]->AddAction(new Task(task_name, resource_name, 1, 
		resource_names, resource_amounts));
}

void LocationManager::DeleteQuestLocation(QuestLocation* quest_location)
{
	for (int i = 0; i < Quest_Locations_.size(); i++) {
		if (Quest_Locations_[i] == quest_location) {
			delete Quest_Locations_[i];
			Quest_Locations_[i] = nullptr;
			Quest_Locations_.erase(Quest_Locations_.begin() + i);
			return;
		}
	}
}

void LocationManager::DeleteAllTaskLocations()
{
	while (Task_Locations_.size() > 0) {
		delete Task_Locations_.back();
		Task_Locations_.back() = nullptr;
		Task_Locations_.pop_back();
	}
}
