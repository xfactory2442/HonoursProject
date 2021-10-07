#include "StatsAndInventoryScene.h"
#include "ResourceManager.h"

StatsAndInventoryScene::StatsAndInventoryScene()
{
	//Setup the inventory text.
	text_.setFont(GeneralVariables::font_);
	text_.setCharacterSize(20);
	text_.setFillColor(sf::Color::White);
	text_.setPosition(5.0f, 5.0f);
}

bool StatsAndInventoryScene::Update(float dt)
{
	//If escape is pressed then leave the inventory.
	if (Input::GetKeyDown(sf::Keyboard::Escape)) {
		Input::SetKeyDown(sf::Keyboard::Escape);
		SceneManager::ChangeScene(SceneManager::Scene::Main);
		return true;
	}
	return false;
}

void StatsAndInventoryScene::Render()
{
	StartDraw();
	GeneralVariables::window_.draw(text_);
	EndDraw();
}

void StatsAndInventoryScene::OnEnter()
{
	/*On entering the inventory scene, create a string of all of the resources the
	player has collected for the inventory, make it look nice-ish and set it
	as the inventory text.*/
	std::string text_string_;
	for (auto resource : ResourceManager::resources) {
		text_string_ += resource.first + ": " + std::to_string(resource.second) + "\n";
	}
	text_.setString(text_string_);
}
