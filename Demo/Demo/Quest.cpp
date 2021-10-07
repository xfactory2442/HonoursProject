#include "Quest.h"
#include "GeneralVariables.h"
#include "ResourceManager.h"
#include "Input.h"

Quest::Quest(std::string quest_name, std::vector<QuestDetails> Quest_Requirements) :
	ThingsToDoAtLocation()
{
	Quest_Requirements_ = Quest_Requirements;

	setSize(getSize() + sf::Vector2f(0.0f, 22.0f * Quest_Requirements_.size() - 1));

	//Setup all the quest text.
	title_.setString(quest_name);
	std::string details;
	for (auto requirement : Quest_Requirements_) {
		details += std::to_string(requirement.amount_) + " " + requirement.resource_ + "\n";
	}
	setSize(sf::Vector2f(getSize().x,
		getSize().y + Quest_Requirements_.size() * (other_text_size_ + 2)));
	other_text_.setString(details);

	refuse_button_ = new Button("Refuse", sf::Vector2f(), sf::Vector2f(100.0f, 25.0f),
		20, sf::Vector2f(2, 2.5f));
}

bool Quest::DoAction()
{
	//Check if all of the task requirements are doable.
	for (auto task : Quest_Requirements_) {
		if (!ResourceManager::CanAddResource(task.resource_, -task.amount_)) {
			return false;
		}
	}

	for (auto task : Quest_Requirements_) {
		ResourceManager::AddResource(task.resource_, -task.amount_);
	}
	return true;
}

void Quest::RenderExtra()
{
	GeneralVariables::window_.draw(*refuse_button_);
	refuse_button_->RenderButtonText();
}

bool Quest::DeleteButtonPressed()
{
	//If refuse button is pressed, delete the quest.
	if (refuse_button_->Collision(GeneralVariables::window_.mapPixelToCoords(Input::GetMouse()))) {
		return true;
	}
	return false;
}

void Quest::SetButtonPosition(sf::Vector2f position)
{
	sf::Vector2f button_position_ = sf::Vector2f(sf::Vector2f(90.0f, getSize().y - 30.0f));
	refuse_button_->SetPosition(position + button_position_);
}

std::vector<Quest::QuestDetails> Quest::GetRequirements()
{
	return Quest_Requirements_;
}
