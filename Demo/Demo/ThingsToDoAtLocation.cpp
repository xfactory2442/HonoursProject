#include "ThingsToDoAtLocation.h"
#include "GeneralVariables.h"
#include "Input.h"
#include "LocationScene.h"

ThingsToDoAtLocation::ThingsToDoAtLocation() :
	GameObject(sf::Vector2f(298.0f, 50.0f), sf::Vector2f(0.0f, 0.0f), nullptr)
{
	setFillColor(sf::Color(100, 100, 100, 255));

	title_.setFont(GeneralVariables::font_);
	title_.setCharacterSize(30);

	other_text_.setFont(GeneralVariables::font_);
	other_text_.setCharacterSize(other_text_size_);



	do_action_button_ = new Button("Do Action", sf::Vector2f(), sf::Vector2f(100.0f, 25.0f),
		20, sf::Vector2f(2, 2.5f));
}

bool ThingsToDoAtLocation::ButtonPressed()
{
	//Check if a button is pressed and if that action can be done. If not then set a 
	//failure text to show up.
	if (do_action_button_->Collision(GeneralVariables::window_.mapPixelToCoords(Input::GetMouse()))) {
		bool action_complete = DoAction();
		if (!action_complete) {
			LocationScene::failed_action_timer_ = 1.5f;
		}
		return action_complete;
	}
	return false;
}

void ThingsToDoAtLocation::Render()
{
	GeneralVariables::window_.draw(*this);
	GeneralVariables::window_.draw(title_);
	GeneralVariables::window_.draw(other_text_);
	GeneralVariables::window_.draw(*do_action_button_);
	do_action_button_->RenderButtonText();
}

void ThingsToDoAtLocation::SetUpPositionOnScreen(sf::Vector2f position)
{
	setPosition(position);
	title_.setPosition(position + sf::Vector2f(5.0f, 2.0f));
	other_text_.setPosition(position + sf::Vector2f(5.0f, 40.0f));
	do_action_button_->SetPosition(position + sf::Vector2f(195.0f, getSize().y - 30.0f));
}
