#pragma once
#include <SFML/Graphics.hpp>
class Button : public sf::RectangleShape
{
public:
	Button(std::string nameText_, sf::Vector2f position, sf::Vector2f size,
		float fontSize = 15, sf::Vector2f textPosition = sf::Vector2f(2, 5),
		sf::Color color = sf::Color::White, sf::Color textColor = sf::Color::Black);
	bool Collision(sf::Vector2f mousePosition);
	void RenderButtonText();

	void SetPosition(sf::Vector2f position_);

	std::string GetButtonName();
private:
	sf::Text nameText;
	sf::FloatRect hitbox;
};

