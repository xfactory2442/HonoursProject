#include "GameObject.h"
#include "GeneralVariables.h"

GameObject::GameObject(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture,
	GameObject* parent, bool children_render_children)
{
	setSize(size);
	setPosition(position);
	setTexture(texture);
	parent_ = parent;
	children_render_children_ = children_render_children;
}

void GameObject::AddChild(GameObject* object)
{
	if (std::find(Children_.begin(), Children_.end(), object) !=
		Children_.end()) {
		Children_.push_back(object);
	}
}

void GameObject::MoveObjectAndChildrenToPosition(sf::Vector2f new_position)
{
	//Move this and any object connected to it together.
	for (auto child : Children_) {
		child->setPosition(new_position - (getPosition() - child->getPosition()));
	}
	setPosition(new_position);
}
