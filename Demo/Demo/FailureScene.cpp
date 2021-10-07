#include "FailureScene.h"
#include "Input.h"
#include "SceneManager.h"

FailureScene::FailureScene()
{
	//Initialise the return to menu button.
	sf::Vector2f button_size(200.0f, 90.0f);
	to_menu_button_ = new Button("To Menu",
		sf::Vector2f(GeneralVariables::window_.getSize().x / 2.0f - button_size.x / 2.0f,
			GeneralVariables::window_.getSize().y / 4.0f * 3.0f - button_size.y / 2.0f),
		button_size, 50, sf::Vector2f(20.0f, 5.0f));
}

bool FailureScene::Update(float dt)
{
	//If the return to menu button is pressed then return to the menu.
	if (Input::GetMouseLeftDown() &&
		to_menu_button_->Collision(GeneralVariables::window_.mapPixelToCoords(Input::GetMouse()))) {
		Input::SetMouseLeftDown(false);
		SceneManager::ChangeScene(SceneManager::Scene::Menu);
		return true;
	}
	return false;
}

void FailureScene::Render()
{
	StartDraw();
	GeneralVariables::window_.draw(*to_menu_button_);
	to_menu_button_->RenderButtonText();
	EndDraw();
}
