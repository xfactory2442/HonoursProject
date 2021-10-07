#pragma once
#include "ThingsToDoAtLocation.h"

class Task : public ThingsToDoAtLocation
{
public:
	struct TaskDetails {
		//What item the task will give or what action the task will complete.
		std::string thing_collected_ = "";
		//How much of an item the task will give or how many times the action is completed.
		unsigned int collected_per_task_ = 1;
		//What item is required to complete the task.
		std::vector<std::string> thing_required_;
		//How much of that item is required to complete the task.
		std::vector<int> required_per_task_;
	};
public:
	Task(std::string task_name, std::string thing_collected, unsigned int thing_per_task,
		std::vector<std::string> thing_required = std::vector<std::string>(), 
		std::vector<int> required_per_task = std::vector<int>());
	std::string GetTaskName();
	bool DoAction();
	void RenderExtra();

	std::string GetThingCollected();

private:
	bool IsCompleteable();
	//The name of the task. This is used for quest generation purposes.
	std::string task_name_;
	TaskDetails task_details_;
	
};

