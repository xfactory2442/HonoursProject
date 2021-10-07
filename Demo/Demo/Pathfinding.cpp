#include "Pathfinding.h"
#include "Grid.h"
#include <iostream>

Node* Pathfinding::SetupNode(Node* node, std::vector<Node*> Nodes_)
{
	//Make a map of all the positions around the node.
	std::unordered_map<int, sf::Vector2i> Surrounding_positions;
	Surrounding_positions.insert(std::pair<int, sf::Vector2i>
		(0, sf::Vector2i(0, -1) + node->GetGridPosition()));
	Surrounding_positions.insert(std::pair<int, sf::Vector2i>
		(1, sf::Vector2i(1, 0) + node->GetGridPosition()));
	Surrounding_positions.insert(std::pair<int, sf::Vector2i>
		(2, sf::Vector2i(0, 1) + node->GetGridPosition()));
	Surrounding_positions.insert(std::pair<int, sf::Vector2i>
		(3, sf::Vector2i(-1, 0) + node->GetGridPosition()));

	//If a node neighbours the new node, make them neighbours.
	for (int i = 0; i < Nodes_.size(); i++) {
		if (Nodes_[i]->GetNumNeighbours() != 4) {
			for (auto& position : Surrounding_positions) {
				if (Nodes_[i]->GetGridPosition() == position.second) {
					node->AddNeighbour(Nodes_[i], position.first);
					int r_index = position.first + 2;
					r_index = r_index > 3 ? r_index - 4 : r_index;
					Nodes_[i]->AddNeighbour(node, r_index);
					Surrounding_positions.erase(position.first);
					break;
				}
			}
			if (node->GetNumNeighbours() == 4) {
				break;
			}
		}
	}
	Nodes_.push_back(node);

	return node;
}

void Pathfinding::FindAvailableNodes(Character* character, std::vector<Node*> Nodes_)
{
	//Empty the moveable nodes variable.
	character->Moveable_Nodes_.clear();

	//Set the max distance for the pathfinder function.
	character->Moveable_Nodes_.push_back(character->GetGridNode());
	//Set up a vector of nodes that need to be checked.
	Nodes_To_Be_Checked_ = Nodes_;
	while (Nodes_To_Be_Checked_.size() > 0) {
		//Pathfind to current node.
		Node* node = Nodes_To_Be_Checked_.back();
		Nodes_To_Be_Checked_.pop_back();
		std::vector<Node*> Path = Pathfind(character->GetGridNode(), node, 
			Nodes_, character->GetMovement());

		if (Path.size() > 0) {
			character->Moveable_Nodes_.push_back(node);
		}
		for (int i = 0; i < Path.size(); i++) {
			//If the node in the path isnt already a moveable node.
			if (std::find(character->Moveable_Nodes_.begin(), character->Moveable_Nodes_.end(),
				Path[i]) == character->Moveable_Nodes_.end()) {
				//Add the node to the moveable nodes vector.
				character->Moveable_Nodes_.push_back(Path[i]);
				//Remove the node from the nodes to be checked vector.
				std::vector<Node*>::iterator it = std::find(Nodes_To_Be_Checked_.begin(),
					Nodes_To_Be_Checked_.end(), Path[i]);
				if (it != Nodes_To_Be_Checked_.end()) {
					Nodes_To_Be_Checked_.erase(it);
				}
			}
		}
	}
}

std::vector<Node*> Pathfinding::Pathfind(Node* start_node, Node* end_node, std::vector<Node*> Nodes_, int max_distance)
{

	//
	//A* algorithm.
	//

	//The vector which will hold the final path.
	std::vector<Node*> Path;

	//The vector which will hold the end points position.
	sf::Vector2i end = end_node->GetGridPosition();

	if (!start_node || !end_node || start_node == end_node) {
		return Path;
	}

	//
	//Create the different vectors and maps for the algorithm.
	//

	std::vector<Node*> Open_Set;
	Open_Set.push_back(start_node);

	std::map<Node*, Node*> Previous_Node;

	std::map<Node*, float> gScore;
	gScore.insert(std::pair<Node*, float>(start_node, 0.0f));
	std::map<Node*, float> fScore;
	gScore.insert(std::pair<Node*, float>(start_node, 
		h(start_node->GetGridPosition(), end)));

	while (Open_Set.size() > 0) {
		Node* current = Open_Set[0];
		//Find the node with the lowest fScore and current to it.
		for (int i = 1; i < Open_Set.size(); i++) {
			if (fScore[current] > fScore[Open_Set[i]]) {
				current = Open_Set[i];
			}
		}

		//If current is the end node.
		if (current == end_node) {

			//Reconstruct the path by making a vector of the nodes to visit to reach there.
			Path.push_back(current);
			while (true) {
				if (Previous_Node[current] == start_node) {
					return Path;
				}
				Path.insert(Path.begin(), Previous_Node[current]);
				current = Previous_Node[current];
			}
		}

		//Delete the current node from the open set.
		Open_Set.erase(std::find(Open_Set.begin(), Open_Set.end(), current));

		for (int i = 0; i < 4; i++) {
			Node* neighbour = current->GetNeighbour(i);
			//If the neighbour exists.
			if (neighbour) {
				//Calculate the currentG by adding the gScore for the current node and the
				//distance between current node and neighbour node.
				float currentG = gScore[current] + d(neighbour);
				if (gScore.find(neighbour) == gScore.end()) {
					gScore.insert(std::pair<Node*, float>(neighbour, 1000));
				}
				//If currentG is better than previous g for the neighbour.
				if (currentG < gScore[neighbour] && currentG <= max_distance) {
					Previous_Node[neighbour] = current;
					//If there is not already an entry for that node then create it and set it.
					gScore[neighbour] = (float)h(neighbour->GetGridPosition(), end);
					//If the fScore for that node already exists then set it to it,
					//otherwise create it and then set it.
					fScore[neighbour] =
						currentG + h(neighbour->GetGridPosition(), end);
					//If the neighbour isnt already in the open set then add it.
					if (std::find(Open_Set.begin(), Open_Set.end(), neighbour) == Open_Set.end()) {
						Open_Set.push_back(neighbour);
					}
				}
			}
		}
	}
	Path.clear();
	return Path;
}

Node* Pathfinding::FindNodeByPosition(sf::Vector2i grid_position, std::vector<Node*> Nodes_)
{
	for (int i = 0; i < Nodes_.size(); i++) {
		if (Nodes_[i]->GetGridPosition() == grid_position) {
			return Nodes_[i];
		}
	}
	return nullptr;
}

int Pathfinding::FindNodeNumByPosition(sf::Vector2i grid_position, std::vector<Node*> Nodes_)
{
	for (int i = 0; i < Nodes_.size(); i++) {
		if (Nodes_[i]->GetGridPosition() == grid_position) {
			return i;
		}
	}
	return -1;
}


int Pathfinding::h(sf::Vector2i start, sf::Vector2i end)
{
	return abs(start.x - end.x) + abs(start.y - end.y);
}

int Pathfinding::d(Node* end)
{
	return end->GetDistance();
}