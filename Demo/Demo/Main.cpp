#include <SFML/Graphics.hpp>
#include <ctime>
#include <stdlib.h> 
#include <fstream>
#include <time.h>
#include <stdio.h>
#include "Input.h"
#include "Grid.h"
#include "SceneManager.h"
#include "TurnManager.h"
#include "GeneralVariables.h"
#include "ResourceManager.h"
#include "AnimationManager.h"

#include "MenuScene.h"
#include "MainScene.h"
#include "LocationScene.h"
#include "FailureScene.h"
#include "StatsAndInventoryScene.h"

#include "InfoWindow.h"

//
//Declaring Static Members
//

//Input static members.
bool Input::keys[256];
Input::Mouse Input::mouse;

//SceneManager static members.
SceneManager::Scene SceneManager::scene_ = SceneManager::Scene::Menu;
Location* SceneManager::current_location_ = nullptr;
bool SceneManager::scene_changed_ = false;

//Grid static members.
const float Grid::grid_spacing_ = 50.0f;
sf::View Grid::grid_view_;

//Turn Manager static members.
TurnManager::Turn TurnManager::turn_ = TurnManager::Turn::Player;
int TurnManager::character_turn_num_ = 0;
Character* TurnManager::character_turn_ = nullptr;
CharacterManager* TurnManager::character_manager_ = nullptr;
bool TurnManager::was_moveable_ = false;
int TurnManager::turn_num_ = 0;
const float TurnManager::max_turn_timer_ = 0.3f;
float TurnManager::turn_timer_ = 0.0f;

//General Variables static members.
sf::Vector2u window_size_(800, 800);
sf::RenderWindow GeneralVariables::window_(sf::VideoMode(window_size_.x, window_size_.y), "Demo");
sf::Font GeneralVariables::font_;

//Resource Manager static members.
std::unordered_map<std::string, unsigned int> ResourceManager::resources;

//Location Scene static members.
float LocationScene::failed_action_timer_ = 0.0f;

//AnimationManager static members.
std::function<bool(float)> AnimationManager::current_animation_ = [](float dt) {return false; };
std::vector<std::function<bool(float)>> AnimationManager::Animation_List_;

int main() {

	srand((int)time(NULL));

	//Initialise Clock Object.
	sf::Clock clock;

	//Initialise Input.
	Input::Init();

	//Load font in from file.
	GeneralVariables::font_.loadFromFile("fonts/consola.ttf");

	//Initialise the info window.
	InfoWindow info_window_;

	//Initialise Scene Objects.
	MenuScene menu_scene_;
	MainScene main_scene_(&info_window_);
	LocationScene location_scene_(main_scene_.GetCharacterManager(), &info_window_);
	FailureScene failure_scene_;
	StatsAndInventoryScene stats_and_inventory_scene_;

	while (GeneralVariables::window_.isOpen())
	{
		sf::Event event;
		while (GeneralVariables::window_.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				GeneralVariables::window_.close();
				break;
			case sf::Event::Resized:
				GeneralVariables::window_.setSize(window_size_);
				break;
			case sf::Event::KeyPressed:
				// update input class
				Input::SetKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				//update input class
				Input::SetKeyUp(event.key.code);
				break;
			case sf::Event::MouseMoved:
				//update input class
				Input::SetMouseXPosition(event.mouseMove.x);
				Input::SetMouseYPosition(event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					Input::SetMouseLeftDown(true);
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					//update input class
					Input::SetMouseRightDown(true);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					Input::SetMouseLeftDown(false);
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					//update input class
					Input::SetMouseRightDown(false);
				}
				break;
			default:
				// don't handle other events
				break;
			}
		}
		float dt = clock.restart().asSeconds();

		switch (SceneManager::GetScene()) {
		case SceneManager::Scene::Menu:
			if (menu_scene_.Update(dt)) {
				main_scene_.Init();
			}
			menu_scene_.Render();
			break;
		case SceneManager::Scene::Main:
			main_scene_.Update(dt);
			main_scene_.Render();
			break;
		case SceneManager::Scene::Location:
			location_scene_.Update(dt);
			location_scene_.Render();
			break;
		case SceneManager::Scene::Failure:
			failure_scene_.Update(dt);
			failure_scene_.Render();
			break;
		case SceneManager::Scene::StatsAndInventory:
			stats_and_inventory_scene_.Update(dt);
			stats_and_inventory_scene_.Render();
		}

		if (SceneManager::scene_changed_) {
			SceneManager::scene_changed_ = false;
			switch (SceneManager::GetScene()) {
			case SceneManager::Scene::Location:
				location_scene_.OnEnterLocation();
				break;
			case SceneManager::Scene::StatsAndInventory:
				stats_and_inventory_scene_.OnEnter();
				break;
			case SceneManager::Scene::Failure:
				main_scene_.Release();
				break;
			}

		}
	}
}