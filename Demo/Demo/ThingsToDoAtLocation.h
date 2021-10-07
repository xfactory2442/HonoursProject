#pragma once
#include "GameObject.h"
#include "Button.h"

class ThingsToDoAtLocation : public GameObject
{
public:
	ThingsToDoAtLocation();
	virtual bool DoAction() = 0;
	bool ButtonPressed();
	void Render();
	virtual void RenderExtra() = 0;

	void SetUpPositionOnScreen(sf::Vector2f position);

protected:
	sf::Text title_;
	sf::Text other_text_;
	const unsigned int other_text_size_ = 20;
	Button* do_action_button_ = nullptr;
};

