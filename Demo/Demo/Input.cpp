#include "Input.h"

//Sets all keys that are pressed down.
void Input::SetKeyDown(int key)
{
	if (key >= 0) {
		keys[key] = true;
	}
}

void Input::SetKeyUp(int key)
{
	if (key >= 0) {
		keys[key] = false;
	}
}

//Set the X Coordinate of the mouse.
void Input::SetMouseXPosition(int x)
{
	mouse.x = x;
}

//Set the Y Coordinate of the mouse.
void Input::SetMouseYPosition(int y)
{
	mouse.y = y;
}

//Set if the left mouse button is pressed down.
void Input::SetMouseLeftDown(bool down)
{
	mouse.left = down;
}

//Set if the right mouse button is pressed down.
void Input::SetMouseRightDown(bool down)
{
	mouse.right = down;
}

//Get the position if the mouse.
sf::Vector2i Input::GetMouse() {
	return sf::Vector2i(mouse.x, mouse.y);
}

bool Input::GetMouseLeftDown()
{
	return mouse.left;
}

bool Input::GetKeyDown(int key)
{
	return keys[key];
}

void Input::Init()
{
	for (int i = 0; i < 256; i++) {
		keys[i] = false;
	}
}
