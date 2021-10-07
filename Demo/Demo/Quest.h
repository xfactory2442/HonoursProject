#pragma once
#include <string>
#include <vector>
#include "ThingsToDoAtLocation.h"

class Quest : public ThingsToDoAtLocation
{
public:
	struct QuestDetails {
		std::string resource_;
		int amount_ = 0;
	};
	Quest(std::string quest_name, std::vector<QuestDetails> Quest_Requirements);
	bool DoAction();
	void RenderExtra();

	bool DeleteButtonPressed();
	void SetButtonPosition(sf::Vector2f position);

	std::vector<QuestDetails> GetRequirements();

private:
	std::vector<QuestDetails> Quest_Requirements_;
	Button* refuse_button_ = nullptr;
};

