#pragma once
#include "GameObject.h"

class Icon : public GameObject
{
public:
	Icon(sf::Vector2f position, sf::Vector2f size);
	void SetText(std::string text);
	void Render();
private:
	sf::Text text_;
};

