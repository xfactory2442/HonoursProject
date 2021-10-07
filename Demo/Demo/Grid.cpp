#include "Grid.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include "GeneralFunctions.h"
#include "SceneManager.h"
#include "Enemy.h"
#include "GeneralVariables.h"
#include "TaskLocation.h"
#include "AnimationManager.h"

Grid::Grid(LocationManager* location_manager)
{
	location_manager_ = location_manager;
	location_texture_.loadFromFile("gfx/exclamationMark.png");

	//Load grid textures from file.
	Main_Textures_.push_back(LoadTexture("gfx/grassyPlane.png"));
	Main_Textures_.push_back(LoadTexture("gfx/sandyPlane.png"));
	Main_Textures_.push_back(LoadTexture("gfx/stonePlane.png"));
	Sub_Textures_.insert(std::pair<string, sf::Texture*>("hut", LoadTexture("gfx/hut.png")));
	Sub_Textures_.insert(std::pair<string, sf::Texture*>("mountain", LoadTexture("gfx/loneMountain.png")));

	CreateMap("maps/test.csv");
}

Grid::~Grid()
{
}

bool Grid::InitialiseCharacter(Character* character, sf::Vector2i node_position)
{
	Node* node = pathfinding_.FindNodeByPosition(node_position, Nodes_);
	return InitialiseCharacter(character, node);
}

bool Grid::InitialiseCharacter(Character* character, Node* node)
{
	//Set the character on the tile and find their movement area.
	if (!node->GetCharacterOnTile() && !node->GetLocation()) {
		SetCharacterPositionOnGrid(character, node);
		pathfinding_.FindAvailableNodes(character, Nodes_);
		return true;
	}
	return false;
}

bool Grid::MoveCharacter(Character* character, Node* node)
{
	//If the player can move to that node, pathfind to there.
	if (character && node && !node->GetCharacterOnTile() &&
		std::find(character->Moveable_Nodes_.begin(), character->Moveable_Nodes_.end(),
		node) != character->Moveable_Nodes_.end()) {
		movement_nodes_ = pathfinding_.Pathfind(character->GetGridNode(), node, Nodes_);
		SetMovingCharacter(character);
		previous_movement_node_ = character->GetGridNode();
		return true;
	}
	return false;
}

void Grid::MoveCharacter(Character* character, sf::Vector2i node_position)
{
	Node* node = pathfinding_.FindNodeByPosition(node_position, Nodes_);
	MoveCharacter(character, node);
}

void Grid::MoveCharacterTowardsTarget(Character* character, GridObject* target)
{
	//Find the path between the two points. max_distance == 1000 because its irrelevant.
	std::vector<Node*> Path = pathfinding_.Pathfind(character->GetGridNode(),
		target->GetGridNode(), Nodes_, 1000);
	//The enemy cannot stand on top of the npc/player so make sure they are not going to.
	if (Path.size() > 0) {
		Node* node_position = nullptr;
		do {
			if (target->isOverlap()) {
				node_position = Path[Path.size() - 1];
			}
			else if (Path.size() > 1) {
				node_position = Path[Path.size() - 2];
			}
			Path.pop_back();
		} while (Path.size() > 0 && node_position && !MoveCharacter(character, node_position));
	}
}

Character* Grid::GetMovingCharacter()
{
	return moving_character_;
}

bool Grid::MovementAnimation(float dt)
{
	//If there is a character which should be moving then animate them moving.
	if (!moving_character_) {
		return false;
	}
	if (movement_nodes_.size() > 0) {
			movement_timer_ += dt;
			moving_character_->MoveObject(GeneralFunctions::LerpVector2(
				previous_movement_node_->getPosition(),
				movement_nodes_.front()->getPosition(),
				movement_timer_ / max_movement_timer_));
			if (movement_timer_ >= max_movement_timer_) {
				movement_timer_ = 0.0f;
				previous_movement_node_ = movement_nodes_.front();
				movement_nodes_.erase(movement_nodes_.begin());
			}
		}
	else {
		//Set the character on their final node and stop their movement.
		SetCharacterPositionOnGrid(moving_character_, previous_movement_node_);
		pathfinding_.FindAvailableNodes(moving_character_, Nodes_);
		std::vector<std::string> memory;
		memory.push_back("Moved");
		moving_character_->AddMemory(memory);
		moving_character_ = nullptr;
		movement_timer_ = 0.0f;
	}
	return true;
}

Node* Grid::GridCollision(sf::Vector2f mouse_position)
{
	//If a node is clicked on then resolve that click.
	for (int i = 0; i < Nodes_.size(); i++) {
		if (Nodes_[i]->Collision(mouse_position)) {
			return Nodes_[i];
		}
	}
	return NULL;
}

void Grid::CreateMap(std::string file_name)
{
	//
	//Reading the map from file and building it.
	//

	std::fstream file;
	file.open(file_name);
	std::string line, word, temp;

	int highestX = 0;
	int highestY = 0;

	while (file >> temp) {
		std::vector<std::string> row;

		std::getline(file, line);
		temp += line;

		std::stringstream s(temp);

		while (std::getline(s, word, ',')) {
			row.push_back(word);
		}

		if (row.size() > 2) {
			int positionX = std::stoi(row[0]);
			int positionY = std::stoi(row[1]);
			sf::Texture* main_texture = Main_Textures_[std::stoi(row[2])];
			sf::Texture* sub_texture = NULL;
			if (row.size() > 3 && Sub_Textures_.find(row[3]) != Sub_Textures_.end()) {
				sub_texture = Sub_Textures_[row[3]];
			}
			Nodes_.push_back(new Node(sf::Vector2i(positionX, positionY), main_texture,
				sub_texture));;
			pathfinding_.SetupNode(Nodes_.back(), Nodes_);

			if (highestX < positionX) {
				highestX = positionX;
			}
			if (highestY < positionY) {
				highestY = positionY;
			}
		}
	}

	grid_size_.x = highestX;
	grid_size_.y = highestY;
}

void Grid::AddLocation(sf::Vector2i node_position, Location* location)
{
	Node* node = pathfinding_.FindNodeByPosition(node_position, Nodes_);
	AddLocation(node, location);
}

void Grid::AddLocation(Node* node, Location* location)
{
	if (node) {
		node->SetLocation(location);
	}
}

bool Grid::CheckIfLocation(Node* current_node)
{
	//If there is a location on that node then enter it.
	Location* location = current_node->GetLocation();
	if (location) {
		SceneManager::ChangeScene(SceneManager::Scene::Location);
		SceneManager::SetLocation(location);
		return true;
	}
	return false;
}

bool Grid::CheckIfInRange(Node* node1, Node* node2, int max_distance)
{
	//Check if the two nodes are in range of each other.
	int distanceX = abs(node1->GetGridPosition().x - node2->GetGridPosition().x);
	int distanceY = abs(node1->GetGridPosition().y - node2->GetGridPosition().y);

	if (distanceX + distanceY <= max_distance) {
		return true;
	}
	return false;
}

Character* Grid::FindClosestTarget(Character* character)
{
	Character* closest_object = nullptr;
	float closest_distance_ = 10000.0f;
	//For every node, check if there is a character on that tile.
	for (int i = 0; i < Nodes_.size(); i++) {
		Character* character2 = Nodes_[i]->GetCharacterOnTile();
		if (character2 && !character2->isEnemy()) {
			//Get the distance between the character and the enemy.
			float distance = GeneralFunctions::DistanceBetweenTwoObjects(
				character->GetGridNode()->GetGridPosition(), character2->GetGridNode()->GetGridPosition());
			//If the character is closer than the current closest set the character to
			//be the current closest.
			if (distance < closest_distance_) {
				closest_distance_ = distance;
				closest_object = character2;
			}
		}
	}
	return closest_object;
}

Node* Grid::GetNodeAtPositionOrClosest(const sf::Vector2i node_position)
{
	//Get the exact node inputted or the closest node available if it doesnt exist.
	Node* closest_node = Nodes_[0];
	int distance = (int)sqrtf(
		pow(float(node_position.x - Nodes_[0]->GetGridPosition().x), 2.0f) +
		pow(float(node_position.y - Nodes_[0]->GetGridPosition().y), 2.0f)
	);
	for (int i = 1; i < Nodes_.size(); i++) {
		int new_distance = (int)sqrtf(
			pow(float(node_position.x - Nodes_[i]->GetGridPosition().x), 2.0f) +
			pow(float(node_position.y - Nodes_[i]->GetGridPosition().y), 2.0f)
		);
		if (new_distance < distance) {
			closest_node = Nodes_[i];
			if (new_distance == 0) {
				return closest_node;
			}
			distance = new_distance;
		}
	}
	return closest_node;
}

Node* Grid::GetRandomNodeWithoutCharacter()
{
	Node* node = nullptr;
	while (!node) {
		int random = rand() % Nodes_.size();
		if (!Nodes_[random]->GetCharacterOnTile()) {
			node = Nodes_[random];
		}
	}

	return node;
}

void Grid::RenderGridPieces()
{
	for (int i = 0; i < Nodes_.size(); i++) {
		GeneralVariables::window_.draw(*Nodes_[i]);
		Nodes_[i]->Render();
	}
}

void Grid::RenderLocationIndicators()
{
	for (int i = 0; i < Nodes_.size(); i++) {
		Location* location = Nodes_[i]->GetLocation();
		if (location) {
			location->RenderIndicatorOnMap();
		}
	}
}

void Grid::SetGridObjectPositionOnGrid(GridObject* game_object, sf::Vector2i position_on_grid)
{
	//Find the node for that game_object.
	Node* node = pathfinding_.FindNodeByPosition(position_on_grid, Nodes_);

	SetGridObjectPositionOnGrid(game_object, node);
}

void Grid::SetGridObjectPositionOnGrid(GridObject* game_object, Node* target_node)
{
	//If the node exists then set the grid object on it.
	if (target_node) {
		game_object->setPosition(sf::Vector2f(
			target_node->GetGridPosition().x * grid_spacing_ + grid_spacing_ / 2.0f,
			target_node->GetGridPosition().y * grid_spacing_ + grid_spacing_ / 2.0f));
		game_object->SetGridNode(target_node);
	}
}

void Grid::SetCharacterPositionOnGrid(Character* character, sf::Vector2i position_on_grid)
{
	//Find the node for that game_object.
	Node* node = pathfinding_.FindNodeByPosition(position_on_grid, Nodes_);
	SetCharacterPositionOnGrid(character, node);
}

void Grid::SetCharacterPositionOnGrid(Character* character, Node* target_node)
{
	//Remove the character from their old node.
	if (character->GetGridNode()) {
		character->GetGridNode()->SetCharacterOnTile(nullptr);
	}
	//Set the character on the new node.
	if (target_node) {
		character->SetGridNode(target_node);
		target_node->SetCharacterOnTile(character);
	}
}

sf::Texture* Grid::LoadTexture(std::string texture_name)
{
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile(texture_name);
	
	return texture;
}

void Grid::SetMovingCharacter(Character* character)
{
	moving_character_ = character;
	AnimationManager::Animation_List_.push_back([this](float dt) { return MovementAnimation(dt); });
}
