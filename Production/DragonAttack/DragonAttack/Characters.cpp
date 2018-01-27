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
	:removed{ false }, damage{ 0 }, moving{ false }, direction{ 0 }
{
}

boolean Characters::IsRemoved() const
{
	return removed;
}

void Characters::SetRemoved(boolean dead)
{
	if (this->IsRemoved()) //checks if its alraedy dead
		return;

	removed = dead;
}

boolean Characters::IsMoving() const
{
	return moving;
}

void Characters::SetMoving(boolean movement)
{
	moving = movement;
}

void Characters::SetDamage(int attack)
{
	damage = attack;
}

int Characters::GetDamage() const
{
	return damage;
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