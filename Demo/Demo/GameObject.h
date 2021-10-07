#pragma once
#include "SFML/Graphics.hpp"

class GameObject : public sf::RectangleShape
{
public:
	GameObject(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture,
		GameObject* parent = nullptr, bool children_render_children = true);
	void AddChild(GameObject* object);
	void MoveObjectAndChildrenToPosition(sf::Vector2f new_position);

protected:
	bool children_render_children_ = true;
	std::vector<GameObject*> Children_;
	GameObject* parent_ = nullptr;
};

