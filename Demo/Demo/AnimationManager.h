#pragma once
#include "SFML/Graphics.hpp"
#include <functional>

class AnimationManager
{
public:
	static std::vector<std::function<bool(float)>> Animation_List_;
	static bool RunCurrentAnimation(float dt);

private:
	static std::function<bool(float)> current_animation_;
};

