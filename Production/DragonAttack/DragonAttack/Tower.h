#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Dragon.h"
#include "Create_Object.h"

// A class that creates a basic character
class Tower : public GameObject
{
private:

public:
	Tower(float x, float y); // default constructor

	void Update(Dragon &player, const float &dt);
	void Update(Characters &obj, const float &dt);
};
