#pragma once
#include "Input.h"
#include "Button.h"
#include "Pathfinding.h"
#include <map>
#include "LocationManager.h"

class Enemy;

class Grid
{
public:
	Grid(LocationManager* location_manager);
	~Grid();

	bool InitialiseCharacter(Character* character, sf::Vector2i node_position);
	bool InitialiseCharacter(Character* character, Node* node);
	bool MoveCharacter(Character* character, Node* node);
	void MoveCharacter(Character* character, sf::Vector2i node_position);
	void MoveCharacterTowardsTarget(Character* character, GridObject* target);
	Character* GetMovingCharacter();
	bool MovementAnimation(float dt);
	Node* GridCollision(sf::Vector2f mouse_position);
	bool CheckIfInRange(Node* node1, Node* node2, int max_distance);

	void CreateMap(std::string file_name);
	void AddLocation(sf::Vector2i node_position, Location* location);
	void AddLocation(Node* node, Location* location);
	bool CheckIfLocation(Node* current_node);

	void SetGridObjectPositionOnGrid(GridObject* game_object, sf::Vector2i position_on_grid);
	void SetGridObjectPositionOnGrid(GridObject* game_object, Node* target_node);
	void SetCharacterPositionOnGrid(Character* character, sf::Vector2i position_on_grid);
	void SetCharacterPositionOnGrid(Character* character, Node* target_node);

	//Find closest target for the enemy.
	Character* FindClosestTarget(Character* enemy);

	Node* GetNodeAtPositionOrClosest(const sf::Vector2i node_position);
	Node* GetRandomNodeWithoutCharacter();

	void RenderGridPieces();
	void RenderLocationIndicators();
	
	//Static variables.
	//The distance between nodes/size of grid pieces.
	static const float grid_spacing_;

	//The view of the grid so that the camera can be focused on the active character.
	static sf::View grid_view_;

	//The amount of tiles in the x and y directions.
	sf::Vector2i grid_size_;

private:
	sf::Texture* LoadTexture(std::string texture_name);

	LocationManager* location_manager_ = nullptr;

	//The texture for locations.
	sf::Texture location_texture_;

	//
	//Grid piece variables.
	//

	//Nodes vector.
	std::vector<Node*> Nodes_;
	//A vector of all of the main grid piece textures.
	std::vector<sf::Texture*> Main_Textures_;
	//A vector of all of the sub textures for the grid pieces.
	std::map<std::string, sf::Texture*> Sub_Textures_;

	//Pathfinding object.
	Pathfinding pathfinding_;
	void SetMovingCharacter(Character* character);
	//The character which is moving.
	Character* moving_character_ = nullptr;
	//A vector of all the nodes still to be moved to for animation.
	std::vector<Node*> movement_nodes_;
	//The previous node the character visited.
	Node* previous_movement_node_ = nullptr;
	//The maximum time for movement between nodes.
	const float max_movement_timer_ = 0.2f;
	//The time past during movement between nodes.
	float movement_timer_ = 0.0f;
};