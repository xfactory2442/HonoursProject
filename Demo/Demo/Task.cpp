#include "Task.h"
#include "GeneralVariables.h"
#include <string>
#include "ResourceManager.h"

Task::Task(std::string task_name, std::string thing_collected, unsigned int thing_per_task,
	std::vector<std::string> thing_required, std::vector<int> required_per_task) :
	ThingsToDoAtLocation()
{
	task_name_ = task_name;
	task_details_.thing_collected_ = thing_collected;
	task_details_.collected_per_task_ = thing_per_task;
	task_details_.thing_required_ = thing_required;
	task_details_.required_per_task_ = required_per_task;
	
	title_.setString(task_name_);

	//Set the tasks text up by adding all of the task requirements/rewards into a string.
	int lines = 1;
	std::string other_text_string = "Gain " + std::to_string(thing_per_task) + " " + thing_collected + ".";
	if (task_details_.thing_required_.size() > 0) {
		other_text_string += "\nRequired:\n";
		lines++;
		for (int i = 0; i < task_details_.thing_required_.size(); i++) {
			other_text_string += "	" + std::to_string(task_details_.required_per_task_[i])
				+ " " + task_details_.thing_required_[i] + "\n";
			lines++;
		}
	}
	other_text_.setString(other_text_string);

	setSize(sf::Vector2f(getSize().x, getSize().y + (other_text_size_ + 2) * lines));
}

std::string Task::GetTaskName()
{
	return task_name_;
}

bool Task::DoAction()
{
	//If the task is completeable then gain/lose resources.
	if (IsCompleteable()) {
		for (int i = 0; i < task_details_.thing_required_.size(); i++) {
			ResourceManager::AddResource(task_details_.thing_required_[i], 
				-task_details_.required_per_task_[i]);
		}
		ResourceManager::AddResource(task_details_.thing_collected_, task_details_.collected_per_task_);
		return true;
	}
	return false;
}

void Task::RenderExtra()
{
}

std::string Task::GetThingCollected()
{
	return task_details_.thing_collected_;
}

bool Task::IsCompleteable()
{
	//If all of the required resources are in the inventory then return true.
	for (int i = 0; i < task_details_.thing_required_.size(); i++) {
		if (!ResourceManager::CanAddResource(task_details_.thing_required_[i],
			-task_details_.required_per_task_[i])) {
			return false;
		}
	}
	return true;
}
