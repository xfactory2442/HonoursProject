#include "MenuScene.h"
#include "SceneManager.h"

MenuScene::MenuScene()
{
	//Initialise the start button.
	sf::Vector2f button_size(200.0f, 70.0f);
	to_main_button_ = new Button("Start", 
		sf::Vector2f(GeneralVariables::window_.getSize().x / 2.0f - button_size.x / 2.0f,
			GeneralVariables::window_.getSize().y / 4.0f * 3.0f - button_size.y / 2.0f),
		button_size, 50, sf::Vector2f(30.0f, 5.0f));
}

bool MenuScene::Update(float dt)
{
	//If the start button is pressed then start the game.
	if (Input::GetMouseLeftDown() &&
		to_main_button_->Collision(GeneralVariables::window_.mapPixelToCoords(Input::GetMouse()))) {
		Input::SetMouseLeftDown(false);
		SceneManager::ChangeScene(SceneManager::Scene::Main);
		return true;
	}
	return false;
}

void MenuScene::Render()
{
	StartDraw();
	GeneralVariables::window_.draw(*to_main_button_);
	to_main_button_->RenderButtonText();
	EndDraw();
}
