#pragma once
#include "Location.h"
#include "Task.h"
#include "Node.h"

class TaskLocation : public Location
{
public:
	TaskLocation(Node* node, sf::Texture* texture);
	~TaskLocation();
	void AddAction(Task* task);
	void RenderLocation();

	void SetUpLocation();
	bool Update(float dt, Player* player);

private:
	std::vector<Task*> Task_;
	Node* node_ = nullptr;
};

