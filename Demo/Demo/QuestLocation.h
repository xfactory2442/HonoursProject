#pragma once
#include "Location.h"
#include "Quest.h"
#include "NPC.h"
#include "QuestManager.h"

class QuestLocation : public Location
{
public:
	QuestLocation(NPC* npc, Node* node, sf::Texture* texture, QuestManager* quest_manager);
	~QuestLocation();
	void RenderLocation();

	void SetUpLocation();
	bool Update(float dt, Player* player);

private:
	std::string GetRelationshipsText();
	std::string GetNeedsText();

	QuestManager* quest_manager_ = nullptr;

	std::vector<Quest*> Quest_;
	NPC* npc_ = nullptr;
	sf::Text no_quest_text_;
	sf::Text npc_name_text_;
	sf::Text npc_relationships_text_;
	sf::Text npc_needs_text_;
	sf::Text relationship_change_text_;
};

