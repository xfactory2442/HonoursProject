#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "SceneManager.h"
#include "GeneralVariables.h"

class Scene
{
public:
	virtual bool Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	void StartDraw();
	void EndDraw();
};

