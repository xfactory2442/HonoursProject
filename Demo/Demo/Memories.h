#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"
class Memories
{
public:
	struct Memory {
		std::vector<std::string> what_happened_;
		int turns_ago_;
		Node* where_it_happened_;
		int turns_total_;
	};

	void AddMemory(std::vector<std::string> what_happened, Node* where_it_happened);
	void ForgetMemories();
	std::vector<Memories::Memory> GetMemories();

private:
	std::vector<Memory> Memories_;
};

