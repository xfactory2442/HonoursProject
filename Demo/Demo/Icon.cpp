#include "Icon.h"
#include "GeneralVariables.h"

Icon::Icon(sf::Vector2f position, sf::Vector2f size) : 
	GameObject(size, position, nullptr)
{
	text_.setCharacterSize(40);
	text_.setFont(GeneralVariables::font_);
	text_.setFillColor(sf::Color::Black);
	text_.setPosition(position + sf::Vector2f(5.0f, 5.0f));
}

void Icon::SetText(std::string text)
{
	text_.setString(text);
}

void Icon::Render()
{
	GeneralVariables::window_.draw(*this);
	GeneralVariables::window_.draw(text_);
}
