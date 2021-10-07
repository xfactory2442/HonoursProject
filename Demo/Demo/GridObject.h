#pragma once
#include "Node.h"
#include "GeneralVariables.h"
#include "GameObject.h"

class GridObject : public GameObject
{
public:
	GridObject(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture,
		GameObject* parent = nullptr);
	void SetGridNode(Node* grid_position);
	Node* GetGridNode();
	//Can a character stand on top of this thing?
	bool isOverlap();

protected:
	Node* grid_node_ = NULL;
	//Can a character stand on top of this thing?
	bool overlap_ = true;
};

