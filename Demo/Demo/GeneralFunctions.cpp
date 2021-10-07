#include "GeneralFunctions.h"

string GeneralFunctions::FormatNumber(float num)
{
    int numTens = 0;
    if (num > 999999.9f) {
        while (num > 1.0f) {
            num /= 10;
            numTens++;
        }
    }
	string s = to_string(num);
	s.erase(s.begin() + s.size() - 4, s.end());
    if (numTens > 0) {
        s.insert(s.size(), "e" + to_string(numTens));
    }
	return s;
}

string GeneralFunctions::FormatNumber(int num)
{
    int numTens = 0;
    if (num > 999999) {
        while (num > 1) {
            num /= 10;
            numTens++;
        }
    }

    string s = to_string(num);
    if (numTens > 0) {
        s.insert(s.size(), "e" + to_string(numTens));
    }
    return s;
}

string GeneralFunctions::FormatLine(string s, int maxLineSize)
{
    //Takes a string and adds a new line by replacing the last space.

    //Find out the number of lines needed.
    int lines = ceilf((float)s.size() / (float)maxLineSize);

    //The position of the last end line.
    int previousLineEnd = 0;
    for (int i = 0; i < lines - 1; i++) {
        //For the number of characters possible in a line backwards.
        for (int j = maxLineSize; j >= 0; j--) {
            //If the character is blank.
            if (s[previousLineEnd + j] == ' ') {
                //Insert a new line.
                s.insert(previousLineEnd + j + 1, "\n");
                //Set the previous line end to the index of the new line.
                previousLineEnd += j + 1;
                //Repeat for the next line of text.
                break;
            }
        }
    }
    return s;
}

string GeneralFunctions::FormatLine(string s, int maxLineSize, int lines)
{
    //Takes a string and adds a new line by replacing the last space.

    //The position of the last end line.
    int previousLineEnd = 0;
    for (int i = 0; i < lines - 1; i++) {
        //For the number of characters possible in a line backwards.
        for (int j = maxLineSize; j >= 0; j--) {
            //If the character is blank.
            if (s[previousLineEnd + j] == ' ') {
                //Insert a new line.
                s.insert(previousLineEnd + j + 1, "\n");
                //Set the previous line end to the index of the new line.
                previousLineEnd += j + 1;
                //Repeat for the next line of text.
                break;
            }
        }
    }
	return s;
}

string GeneralFunctions::FormatReadInText(string s)
{
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == -123) {
            s.erase(s.begin() + i);
            for (int j = 0; j < 3; j++) {
                s.insert(i, ".");
            }
        }
    }
    return s;
}

int GeneralFunctions::GetIntFromString(std::string s)
{
    for (int i = s.size() - 1; i > -1; i--) {
        if (atoi(&s[i]) > 0) {
            for (int j = i; j > -1; j--) {
                if (atoi(&s[j]) <= 0) {
                    return atoi(&s[j + 1]);
                }
            }
        }
    }
    return 0;
}

float GeneralFunctions::Lerp(float x0, float x1, float t)
{
    return (1 - t) * x0 + t * x1;
}

sf::Vector2f GeneralFunctions::LerpVector2(sf::Vector2f x0, sf::Vector2f x1, float t)
{
    return (1 - t) * x0 + t * x1;
}

float GeneralFunctions::DistanceBetweenTwoObjects(sf::Vector2i position1, sf::Vector2i position2)
{
    return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2));
}
