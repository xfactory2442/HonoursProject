#include "Node.h"
#include "Grid.h"
#include "Character.h"
#include "GeneralVariables.h"

Node::Node(sf::Vector2i grid_position, sf::Texture* main_texture, sf::Texture* added_texture)
{
	//Set the position on the grid for the node and grid piece.
	grid_position_ = grid_position;

	//Set up the main grid piece.
	setTexture(main_texture);
	setSize(sf::Vector2f(Grid::grid_spacing_, Grid::grid_spacing_));
	setOrigin(getSize() / 2.0f);
	setPosition(sf::Vector2f((float)grid_position.x, (float)grid_position.y) * Grid::grid_spacing_);
	if (!main_texture) {
		setFillColor(sf::Color::Cyan);
	}

	//Set up the added grid piece.
	added_.setTexture(added_texture);
	added_.setSize(sf::Vector2f(Grid::grid_spacing_, Grid::grid_spacing_));
	added_.setOrigin(added_.getSize() / 2.0f);
	added_.setPosition(getPosition());

	hitbox_.height = Grid::grid_spacing_;
	hitbox_.width = Grid::grid_spacing_;
	hitbox_.top = getPosition().y - getSize().y / 2.0f;
	hitbox_.left = getPosition().x - getSize().x / 2.0f;

	//Setup the semi-transparent movement indicator.
	moveable_.setSize(sf::Vector2f(Grid::grid_spacing_, Grid::grid_spacing_));
	moveable_.setOrigin(moveable_.getSize() / 2.f);
	moveable_.setPosition(getPosition());
	moveable_.setFillColor(sf::Color(0, 0, 255, 100));
}

void Node::RenderMoveable()
{
	GeneralVariables::window_.draw(moveable_);
}

sf::Vector2i Node::GetGridPosition()
{
	return grid_position_;
}

void Node::AddNeighbour(Node* node, int i)
{
	neighbours_[i] = node;
	num_neighbours_ += 1;
}

Node* Node::GetNeighbour(int i)
{
	return neighbours_[i];
}

int Node::GetNumNeighbours()
{
	return num_neighbours_;
}

int Node::GetDistance()
{
	return distance;
}

bool Node::Collision(sf::Vector2f mouse_position)
{
	if (hitbox_.contains(mouse_position)) {
		return true;
	}
	return false;
}

void Node::SetLocation(Location* location)
{
	location_ = location;
}

Location* Node::GetLocation()
{
	return location_;
}

bool Node::EnterLocation()
{
	if (location_) {
		return true;
	}
	return false;
}

void Node::Render()
{
	if (added_.getTexture()) {
		GeneralVariables::window_.draw(added_);
	}
}

void Node::SetCharacterOnTile(Character* character)
{
	character_on_tile_ = character;
	if (character_on_tile_) {
		character_on_tile_->MoveObject(getPosition());
	}
}

Character* Node::GetCharacterOnTile()
{
	return character_on_tile_;
}
