#pragma once
#include <SFML/Graphics.hpp>
#include "Location.h"

class Character;
class GridPiece;

class Node : public sf::RectangleShape
{
public:
	Node(sf::Vector2i grid_position, sf::Texture* main_texture, sf::Texture* added_texture);
	void RenderMoveable();

	//Get the position of the node in the grid.
	sf::Vector2i GetGridPosition();
	void AddNeighbour(Node* node, int i);
	Node* GetNeighbour(int i);
	int GetNumNeighbours();
	int GetDistance();

	bool Collision(sf::Vector2f mouse_position);
	void SetLocation(Location* location);
	Location* GetLocation();
	bool EnterLocation();

	void Render();

	void SetCharacterOnTile(Character* character);
	Character* GetCharacterOnTile();

private:
	//The position of the node in the grid.
	sf::Vector2i grid_position_ = sf::Vector2i(0, 0);
	//The nodes neighboring this one.
	Node* neighbours_[4] = {NULL, NULL, NULL, NULL};
	//The number of neighbours created so far.
	int num_neighbours_ = 0;
	int distance = 1;

	Character* character_on_tile_ = nullptr;

	//The added texture on top of the background texture.
	sf::RectangleShape added_;
	sf::RectangleShape moveable_;
	//The hitbox for clicking on the piece;
	sf::FloatRect hitbox_;

	Location* location_ = nullptr;

	
};
