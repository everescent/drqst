#pragma once
#include "Characters.h"
#include "Dragon.h"

class Scarecrow : public  Characters
{
private:
public:
	Scarecrow(float x, float y);
	void Update(Dragon &d, const float dt);

};
