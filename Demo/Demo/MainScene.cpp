#include "MainScene.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include "TurnManager.h"
#include "GeneralFunctions.h"
#include "TaskLocation.h"
#include "ResourceManager.h"
#include "AnimationManager.h"

namespace fs = std::filesystem;

MainScene::MainScene(InfoWindow* info_window)
{
	info_window_ = info_window;

	location_manager_ = new LocationManager();
	grid_ = new Grid(location_manager_);
	location_manager_->CreateTaskLocations(grid_);

	ResourceManager::LoadInitialResourcesFromFile(location_manager_);

	//Set up the view.
	grid_->grid_view_.setSize((sf::Vector2f)GeneralVariables::window_.getSize());
	grid_->grid_view_.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	grid_->grid_view_.setCenter(sf::Vector2f(0.0f, 0.0f));

	TurnManager::character_manager_ = &character_manager_;

	quest_manager_ = new QuestManager(&character_manager_);
	character_manager_.SetGrid(grid_);
	character_manager_.SetQuestManager(quest_manager_);
}

MainScene::~MainScene()
{
	Release();
	delete grid_;
	grid_ = nullptr;
}

void MainScene::Init()
{
	//Set the values for the size of the grid pieces and the size of the characters.
	float grid_spacing = Grid::grid_spacing_;

	//Set up player.
	character_manager_.player_ = new Player(5, sf::Vector2f(), nullptr,
		&character_manager_, 1, 1, info_window_);
	character_manager_.player_->setFillColor(sf::Color::Green);
	grid_->InitialiseCharacter(character_manager_.player_, sf::Vector2i(0, 0));
	turn_manager_.character_turn_ = character_manager_.player_;
	turn_manager_.StartTurn(character_manager_.player_);

	info_window_->InitialisePlayer(character_manager_.player_);

	character_manager_.SetLocationManager(location_manager_);

	//Load npcs and enemies from file.
	character_manager_.LoadNPCNamesFromFile();
	for (int i = 0; i < 3; i++) {
		character_manager_.CreateNPC();
	}

	for (auto& p : fs::directory_iterator("Enemies")) {
		character_manager_.LoadEnemyTypesFromFile(p.path().string());
	}

	//Pass in managers into classes as needed.
	turn_manager_.turn_ = TurnManager::Turn::Player;

	int random = rand() % 3 + 1;
	for (int i = 0; i < random; i++) {
		character_manager_.AddEnemySpawnPoint(grid_->GetRandomNodeWithoutCharacter());
	}
}

void MainScene::Release()
{
	location_manager_->DeleteAllQuestLocations();
	delete quest_manager_;
	quest_manager_ = nullptr;
	character_manager_.DeleteAll();
}

bool MainScene::Update(float dt)
{
	//If the info window is active then only handle those inputs.
	if (info_window_->isAlive()) {
		info_window_->Collsion();
		return false;
	}

	if (!TurnManager::TurnTimer(dt)) {
		return false;
	}
	//Move a character if applicable, else do turn actions.
	if (!AnimationManager::RunCurrentAnimation(dt)) {
		if (!turn_manager_.character_turn_) {
			turn_manager_.DetermineCharacterTurn();
		}
		if (turn_manager_.character_turn_) {
			turn_manager_.character_turn_->DoAction(dt, grid_);
			if (turn_manager_.character_turn_) {
				grid_->grid_view_.setCenter(turn_manager_.character_turn_->getPosition());
			}
		}
	}
	else {
		Character* moving_character = grid_->GetMovingCharacter();
		if (moving_character) {
			grid_->grid_view_.setCenter(moving_character->getPosition());
		}
	}
	return false;
}

void MainScene::Render()
{
	StartDraw();

	if (grid_) {
		//Set the view to have the centre on the player.
		GeneralVariables::window_.setView(grid_->grid_view_);

		//Render the grid.
		grid_->RenderGridPieces();
	}

	//Render all of the characters.
	character_manager_.RenderAll();

	//Render the exclamation mark indicators for all locations on top of the moveable areas.
	grid_->RenderLocationIndicators();

	//Reset View
	GeneralVariables::window_.setView(GeneralVariables::window_.getDefaultView());

	RenderUI();
	EndDraw();
}

CharacterManager* MainScene::GetCharacterManager()
{
	return &character_manager_;
}

void MainScene::RenderUI()
{
	//
	//Render the UI elements.
	//

	if (turn_manager_.character_turn_) {
		turn_manager_.character_turn_->RenderIcons();
	}

	//Render the end turn button.
	if (turn_manager_.turn_ == TurnManager::Turn::Player) {
		character_manager_.player_->RenderTurnButton();
	}

	if (info_window_->isAlive()) {
		info_window_->Render();
	}
}
