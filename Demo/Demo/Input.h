#pragma once
#include <SFML/Graphics.hpp>

class Input
{
private:
	// Structure to represent Mouse
	struct Mouse
	{
		int x = 0;
		int y = 0;
		bool left = false;
		bool right = false;
	};

public:

	static void SetKeyDown(int key);
	static void SetKeyUp(int key);
	static void SetMouseXPosition(int x);
	static void SetMouseYPosition(int y);
	static void SetMouseLeftDown(bool down);
	static void SetMouseRightDown(bool down);

	static sf::Vector2i GetMouse();
	static bool GetMouseLeftDown();
	static bool GetKeyDown(int key);
	
	static void Init();

private:
	static bool keys[256];
	static Mouse mouse;
};

