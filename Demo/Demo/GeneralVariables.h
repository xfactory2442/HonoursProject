#pragma once
#include <SFML/Graphics.hpp>
//A place to hold the window and font so they dont have to be passed into every class.
class GeneralVariables
{
public:
	static sf::RenderWindow window_;
	static sf::Font font_;
};

