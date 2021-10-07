#include "Memories.h"
#include <stdlib.h>

void Memories::AddMemory(std::vector<std::string> what_happened, Node* where_it_happened)
{
	//Add a memory to the character based on their actions.
	Memory memory;
	memory.where_it_happened_ = where_it_happened;
	memory.what_happened_ = what_happened;
	memory.turns_ago_ = 0;
	memory.turns_total_ = rand() % 6 + 2;
	Memories_.push_back(memory);
}

void Memories::ForgetMemories()
{
	//Forget old memeories.
	for (int i = 0; i < Memories_.size(); i++) {
		Memories_[i].turns_ago_++;
		if (Memories_[i].turns_ago_ > Memories_[i].turns_total_) {
			Memories_.erase(Memories_.begin() + i);
			i--;
		}
	}
}

std::vector<Memories::Memory> Memories::GetMemories()
{
	return Memories_;
}
