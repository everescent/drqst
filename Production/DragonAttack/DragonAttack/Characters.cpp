/* Start Header ************************************************************************/
/*!
\file       Characters.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Character class, parent class of the characters to be created

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Characters.h"


Characters::Characters()
	:removed{ false }, hp{ 0 }, moving{ false }, direction{ 0 }
{
}

boolean Characters::Is_Removed() const
{
	return removed;
}

void Characters::Set_Removed(boolean dead)
{
	if (this->Is_Removed()) //checks if its alraedy dead
		return;

	removed = dead;
}

boolean Characters::Is_Moving() const
{
	return moving;
}

void Characters::Set_Moving(boolean movement)
{
	moving = movement;
}

void Characters::Set_HP(int hit_points)
{
	hp = hit_points;
}

int Characters::Get_HP() const
{
	return hp;
}

void Characters::Render()
{

}

void Characters::move()
{
}
void Characters::update()
{
}
boolean Characters::collision()
{
	return false;
}