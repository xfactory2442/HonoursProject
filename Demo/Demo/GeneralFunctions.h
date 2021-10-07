#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

/*Most of these functions are not relevant to this program.
This is a class which I transfer between programs and collect reasonably useful
functions that can be used in other places.*/

class GeneralFunctions
{
public:
	static string FormatNumber(float num);
	static string FormatNumber(int num);
	static string FormatLine(string s, int maxLineSize);
	static string FormatLine(string s, int maxLineSize, int lines);
	static string FormatReadInText(string s);
	static int GetIntFromString(std::string s);
	static float Lerp(float start, float end, float time);
	static sf::Vector2f LerpVector2(sf::Vector2f start, sf::Vector2f end, float time);
	static float DistanceBetweenTwoObjects(sf::Vector2i position1, sf::Vector2i position2);
};

