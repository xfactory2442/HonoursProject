#include "TaskLocation.h"
#include "Input.h"
#include "Player.h"

TaskLocation::TaskLocation(Node* node, sf::Texture* texture) :
	Location(node, texture)
{
	node_ = node;
	node_->SetLocation(this);
}

TaskLocation::~TaskLocation()
{
	node_->SetLocation(nullptr);
}

void TaskLocation::AddAction(Task* task)
{
	Task_.push_back(task);
}

void TaskLocation::RenderLocation()
{
	for (int i = 0; i < Task_.size(); i++) {
		Task_[i]->Render();
		Task_[i]->RenderExtra();
	}
}

void TaskLocation::SetUpLocation()
{
	//Set all of the tasks up so they are visible on the screen.
	if (Task_.size() > 0) {
		float sizeY = Task_[0]->getSize().y;
		float positionY = 5.0f;
		for (int i = 0; i < Task_.size(); i++) {
			Task_[i]->SetUpPositionOnScreen(sf::Vector2f(left_size_start_, positionY));
			sizeY = Task_[i]->getSize().y;
			positionY += sizeY + 5.0f;
		}
	}
}

bool TaskLocation::Update(float dt, Player* player)
{
	//If a tasks button is pressed then resolve. 
	for (int i = 0; i < Task_.size(); i++) {
		if (Task_[i]->ButtonPressed()) {
			Input::SetMouseLeftDown(false);
			std::vector<std::string> memory;
			memory.push_back("Task");
			memory.push_back(Task_[i]->GetThingCollected());
			player->AddMemory(memory);
			return true;
		}
	}
	return false;
}
