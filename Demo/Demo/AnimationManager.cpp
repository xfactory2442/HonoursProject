#include "AnimationManager.h"

bool AnimationManager::RunCurrentAnimation(float dt)
{
	if (!current_animation_(dt)) {
		if (Animation_List_.size() > 0) {
			current_animation_ = Animation_List_[0];
			Animation_List_.erase(Animation_List_.begin());
			return true;
		}
		current_animation_ = [](float dt) {return false; };
		return false;
	}
	return true;
}
