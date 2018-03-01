#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Dragon.h"
#include "Create_Object.h"

// A class that creates a basic character
class Wall2 : public GameObject
{
private:

public:
	Wall2(float x, float y); // default constructor

	void Update(Dragon &player, const float &dt);
};
