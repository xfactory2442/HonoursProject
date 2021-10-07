#include "QuestManager.h"
#include "CharacterManager.h"
#include "ResourceManager.h"


QuestManager::QuestManager(CharacterManager* character_manager)
{
    character_manager_ = character_manager;
}

void QuestManager::GenerateQuests(NPC* npc)
{
    /*Determine how many quests the npc should be giving out by their relationship 
    with the player. Weight has to equal at least 1 as the NPC should always 
    be capable of giving out at least one quest*/
    uint32_t weight = npc->GetRelationshipWithCharacter("Player") == 
        0 ? 1 : npc->GetRelationshipWithCharacter("Player");
    if (GetNPCsQuests(npc).size() < (uint32_t)std::ceil(weight / 3)) {
        //Determine what and how many resources should be in the quest.
        std::vector<std::string> Resources = DetermineResources(npc);
        std::vector<Quest::QuestDetails> Quest_Details;
        //Create the quest details.
        for (int i = 0; i < Resources.size(); i++) {
            Quest_Details.push_back(Quest::QuestDetails());
            Quest_Details.back().resource_ = Resources[i];
            if (Quest_Details.back().resource_ != "") {
                Quest_Details.back().amount_ = GetQuestAmount(npc);
                if (Quest_Details.back().amount_ == 0) {
                    Quest_Details.back().amount_++;
                }
            }
        }
        //Add the quest to the npc if it is not empty.
        if (Quest_Details.size() > 0) {
            Quest* quest = new Quest("I need " + Quest_Details[0].resource_ + "!",
                Quest_Details);
            AddQuest(npc, quest);
        }
    }
}

std::vector<Quest*> QuestManager::GetNPCsQuests(NPC* npc)
{
    //If the npc has any quests then return them.
    if (Quests_.find(npc) == Quests_.end()) {
        Quests_.insert(std::pair<NPC*, std::vector<Quest*>>(npc, std::vector<Quest*>()));
    }
    return Quests_[npc];
}

void QuestManager::DeleteQuest(Quest* quest, NPC* npc)
{
    //Delete the inputed quest from the quest list.
    std::vector<Quest*> npc_quests = Quests_[npc];
    std::vector<Quest*>::iterator quest_position = std::find(npc_quests.begin(), npc_quests.end(), quest);
    if (quest_position != npc_quests.end()) {
        npc_quests.erase(quest_position);
    }
    Quests_[npc] = npc_quests;
}

std::vector<std::string> QuestManager::DetermineResources(NPC* npc)
{
    std::vector<std::string> resource;

    //Determine what needs will be added to the quest.
    std::unordered_map<std::string, uint32_t> Needs = npc->GetAllNeeds();
    for (auto& need : Needs) {
        //The higher the need number the more likely random = 0
        int random = rand() % (10 - need.second);
        if (random == 0) {
            //Add resource to list
            resource.push_back(need.first);
        }
    }

    //Determine from previous memories of the player, what resource will be added to the quest.
    std::vector<Memories::Memory> Memories = character_manager_->player_->GetMemories();
    for (int i = (int)Memories.size() - 1; i >= 0; i--) {
        int random = rand() % 2;
        if (Memories[i].what_happened_[0] == "Task" && random == 0) {
            resource.push_back(Memories[i].what_happened_[1]);
            break;
        }
        else if (Memories[i].what_happened_[0] == "Attack" && random == 0) {
            resource.push_back(character_manager_->GetEnemyDropByEnemyName(
                Memories[i].what_happened_[1]));
            break;
        }
    }

    //Randomly cull resources until there are 3 max.
    while (resource.size() > 3) {
        int random = rand() % resource.size();
        resource.erase(resource.begin() + random);
    }
    return resource;
}

void QuestManager::AddQuest(NPC* npc, Quest* quest)
{
    //Add the passed in quest to the npcs list of quests.
    if (Quests_.find(npc) == Quests_.end()) {
        Quests_.insert(std::pair<NPC*, std::vector<Quest*>>(npc, std::vector<Quest*>()));
    }
    Quests_[npc].push_back(quest);
}

unsigned int QuestManager::GetQuestAmount(NPC* npc)
{
    /*Determine how much of an item the npc will ask for depending on
    their relationship with the player*/
    int amount = npc->GetRelationshipWithCharacter("Player") - 9;
    amount = (int)std::ceil(abs(amount) / 3);
    return rand() % 3 + amount;
}
