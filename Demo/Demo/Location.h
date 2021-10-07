#pragma once
#include "SFML/Graphics.hpp"
#include "ThingsToDoAtLocation.h"

class Node;
class LocationManager;
class Player;

class Location
{
public:
	Location(Node* node, sf::Texture* texture);
	void RenderIndicatorOnMap();
	virtual void RenderLocation() = 0;

	virtual void SetUpLocation() = 0;
	virtual bool Update(float dt, Player* player) = 0;

protected:
	const float left_size_start_ = 500.0f;

private:
	sf::RectangleShape indicator_;
};

