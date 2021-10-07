#include "GridObject.h"
#include "GeneralFunctions.h"
#include "GeneralVariables.h"

GridObject::GridObject(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture, GameObject* parent) :
	GameObject(size, position, texture, parent)
{
	setOrigin(size / 2.0f);
}

void GridObject::SetGridNode(Node* grid_position)
{
	grid_node_ = grid_position;
}

Node* GridObject::GetGridNode()
{
	return grid_node_;
}

bool GridObject::isOverlap()
{
	return overlap_;
}
