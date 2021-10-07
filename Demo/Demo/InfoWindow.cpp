#include "InfoWindow.h"
#include "GeneralVariables.h"
#include "Player.h"
#include "Input.h"

InfoWindow::InfoWindow()
{
	//Initialise the info window.
	sf::Vector2f middle_position((sf::Vector2f)GeneralVariables::window_.getSize() / 2.0f);
	setSize(window_size_);
	setOrigin(window_size_ / 2.0f);
	setPosition(middle_position);
	setFillColor(sf::Color(200, 200, 200, 255));

	sf::Vector2f button_size(100.0f, 50.0f);
	//Position = bottom left of info window - button size - a bit more for neatness.
	skip_button_ = new Button("Skip",
		sf::Vector2f(middle_position + window_size_ / 2.0f - button_size
			- sf::Vector2f(10.0f, 10.0f)),
		button_size, 25, sf::Vector2f(13.0f, 10.0f), sf::Color::White);

	health_text_.setCharacterSize(30);
	health_text_.setPosition(skip_button_->getPosition() +
		sf::Vector2f(-170.0f, 8.0f));
	health_text_.setFont(GeneralVariables::font_);

	num_changes_text_.setCharacterSize(50.0f);
	num_changes_text_.setFont(GeneralVariables::font_);
	num_changes_text_.setPosition(sf::Vector2f(
			middle_position.x - window_size_.x / 2.0f + 20.0f,
			middle_position.y - window_size_.y / 2.0f + 10.0f
		));

	InitialiseStatChanges();
}

void InfoWindow::InitialisePlayer(Player* player)
{
	player_ = player;

	//Initialise variables that require the player.
	health_text_.setString(
		"Health: " + std::to_string(player_->GetHealth())
	);

	Stat_Changes_[0].stat_text_.setString(
		"Max Health: " + std::to_string(player_->GetMaxHealth())
	);
	Stat_Changes_[1].stat_text_.setString(
		"Max Movement: " + std::to_string(player_->GetMovement())
	);
	Stat_Changes_[2].stat_text_.setString(
		"Max Movement Actions: " + std::to_string(player_->GetMovementActions())
	);
	Stat_Changes_[3].stat_text_.setString(
		"Max Actions: " + std::to_string(player_->GetActions())
	);
	Stat_Changes_[4].stat_text_.setString(
		"Attack Strength: " + std::to_string(player_->GetAttackStrength())
	);
}

void InfoWindow::Collsion()
{
	if (Input::GetMouseLeftDown()) {
		sf::Vector2f mousePosition = GeneralVariables::window_.mapPixelToCoords(Input::GetMouse());
		//If the skip button is pressed, remove/add 1 health to the player.
		if (skip_button_->Collision(mousePosition)) {
			player_->SubtractHealth(reward_ ? -1 : 1);
			health_text_.setString(
				"Health: " + std::to_string(player_->GetHealth())
			);
			player_->CheckDead();
			IncreaseNumberOfStatChanges(-1);
			if (num_changes_ <= 0) {
				alive_ = false;
			}
			Input::SetMouseLeftDown(false);
			return;
		}
		//Check if any of the stat change buttons are pressed.
		HandleStatChange(mousePosition);
		Input::SetMouseLeftDown(false);
	}
}

void InfoWindow::Render()
{
	if (alive_) {
		GeneralVariables::window_.draw(*this);
		for (auto& stats : Stat_Changes_) {
			if (stats.alive) {
				GeneralVariables::window_.draw(stats.stat_text_);
				GeneralVariables::window_.draw(*stats.button_);
				stats.button_->RenderButtonText();
			}
		}
		GeneralVariables::window_.draw(*skip_button_);
		skip_button_->RenderButtonText();
		GeneralVariables::window_.draw(health_text_);
		GeneralVariables::window_.draw(num_changes_text_);
	}
}

bool InfoWindow::isAlive()
{
	return alive_;
}

void InfoWindow::SetAlive(bool a)
{
	alive_ = a;
}

void InfoWindow::ShowWindow(bool reward, int num)
{
	alive_ = true;
	reward_ = reward;
	IncreaseNumberOfStatChanges(num);

	SetStats();
}

void InfoWindow::SetStats()
{
	if (reward_) {
		Stat_Changes_[0].alive = player_->GetMaxHealth() < 10;
		Stat_Changes_[1].alive = player_->GetMovement() < 8;
		Stat_Changes_[2].alive = player_->GetMaxMovementActions() < 3;
		Stat_Changes_[3].alive = player_->GetMaxActions() < 4;
		Stat_Changes_[4].alive = player_->GetAttackStrength() < 5;
	}
	else {
		Stat_Changes_[0].alive = player_->GetMaxHealth() > 2;
		Stat_Changes_[1].alive = player_->GetMovement() > 1;
		Stat_Changes_[2].alive = player_->GetMaxMovementActions() > 1;
		Stat_Changes_[3].alive = player_->GetMaxActions() > 1;
		Stat_Changes_[4].alive = player_->GetAttackStrength() > 1;
	}
}

void InfoWindow::InitialiseStatChanges()
{
	StatChanges stat_change_;
	float positionY = GeneralVariables::window_.getSize().y / 2.0f 
		- window_size_.y / 2.0f + 65.0f;
	float increase_height = button_size_.y + 5.0f;

	stat_change_ = InitialiseStatChangesDetails(positionY, stat_change_);
	Stat_Changes_.push_back(stat_change_);

	positionY += increase_height;

	stat_change_ = InitialiseStatChangesDetails(positionY, stat_change_);
	Stat_Changes_.push_back(stat_change_);

	positionY += increase_height;

	stat_change_ = InitialiseStatChangesDetails(positionY, stat_change_);
	Stat_Changes_.push_back(stat_change_);

	positionY += increase_height;

	stat_change_ = InitialiseStatChangesDetails(positionY, stat_change_);
	Stat_Changes_.push_back(stat_change_);

	positionY += increase_height;

	stat_change_ = InitialiseStatChangesDetails(positionY, stat_change_);
	Stat_Changes_.push_back(stat_change_);
}

InfoWindow::StatChanges InfoWindow::InitialiseStatChangesDetails(float positionY, 
	StatChanges stat)
{
	float positionXtext = GeneralVariables::window_.getSize().x / 2.0f
		- window_size_.x / 2.0f + 10.0f;
	float positionXbutton = GeneralVariables::window_.getSize().x / 2.0f
		+ window_size_.x / 2.0f - 10.0f - button_size_.x;
	unsigned int character_size = 30;
	stat.stat_text_.setCharacterSize(character_size);
	stat.stat_text_.setFont(GeneralVariables::font_);
	stat.stat_text_.setPosition(positionXtext, positionY);
	//Initialise Button.
	stat.button_ = new Button("Pick", sf::Vector2f(positionXbutton, positionY),
		button_size_, character_size, sf::Vector2f(5.0f, 5.0f));

	return stat;
}

void InfoWindow::HandleStatChange(sf::Vector2f mouse_position)
{
	//If a stat change button is pressed then change that stat and close the window.
	if (Stat_Changes_[0].alive && Stat_Changes_[0].button_->Collision(mouse_position)) {
		player_->ChangeMaxHealth(reward_ ? 1 : -1);
		Stat_Changes_[0].stat_text_.setString(
			"Max Health: " + std::to_string(player_->GetMaxHealth())
		);
		IncreaseNumberOfStatChanges(-1);
	}
	else if (Stat_Changes_[1].alive && Stat_Changes_[1].button_->Collision(mouse_position)) {
		player_->ChangeMovement(reward_ ? 1 : -1);
		Stat_Changes_[1].stat_text_.setString(
			"Max Movement: " + std::to_string(player_->GetMovement())
		);
		IncreaseNumberOfStatChanges(-1);
	}
	else if (Stat_Changes_[2].alive && Stat_Changes_[2].button_->Collision(mouse_position)) {
		player_->ChangeMaxMovementActions(reward_ ? 1 : -1);
		Stat_Changes_[2].stat_text_.setString(
			"Max Movement Actions: " + std::to_string(player_->GetMaxMovementActions())
		);
		IncreaseNumberOfStatChanges(-1);
	}
	else if (Stat_Changes_[3].alive && Stat_Changes_[3].button_->Collision(mouse_position)) {
		player_->ChangeMaxActions(reward_ ? 1 : -1);
		Stat_Changes_[3].stat_text_.setString(
			"Max Actions: " + std::to_string(player_->GetMaxActions())
		);
		IncreaseNumberOfStatChanges(-1);
	}
	else if (Stat_Changes_[4].alive && Stat_Changes_[4].button_->Collision(mouse_position)) {
		player_->ChangeAttackStrength(reward_ ? 1 : -1);
		Stat_Changes_[4].stat_text_.setString(
			"Attack Strength: " + std::to_string(player_->GetAttackStrength())
		);
		IncreaseNumberOfStatChanges(-1);
	}
	if (num_changes_ <= 0) {
		alive_ = false;
	}
	else {
		SetStats();
	}
}

void InfoWindow::IncreaseNumberOfStatChanges(int num)
{
	num_changes_ += num;
	num_changes_text_.setString("Changes: " + std::to_string(num_changes_));
}
