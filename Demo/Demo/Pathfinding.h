#pragma once
#include <unordered_map>
#include "Node.h"
#include "GridObject.h"
#include "Character.h"

class Pathfinding
{
public:
	Node* SetupNode(Node* node, std::vector<Node*> Nodes_);
	void FindAvailableNodes(Character* character, std::vector<Node*> Nodes_);
	std::vector<Node*> Pathfind(Node* start_node, Node* end_node, std::vector<Node*> Nodes_, int max_distance = 1000);
	
	Node* FindNodeByPosition(sf::Vector2i grid_position, std::vector<Node*> Nodes_);
	int FindNodeNumByPosition(sf::Vector2i grid_position, std::vector<Node*> Nodes_);
	
private:
	//Returns the estimated distance between the start and end node.
	int h(sf::Vector2i start, sf::Vector2i end);
	//Returns the distance between the neighbouring nodes.
	int d(Node* end);

	std::vector<Node*> Nodes_To_Be_Checked_;
};

