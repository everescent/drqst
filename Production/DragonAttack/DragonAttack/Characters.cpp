/* Start Header ************************************************************************/
/*!
\file       Characters.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Character class, parent class of the characters to be created

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Characters.h"
#include <iostream>
#include <utility>

 int Characters::score = 0;            // player score for the stage
 int Characters::enemies_killed = 0;   // number of enemies killed

Characters::Characters(Sprite* p_sprite, const int HP, Col_Comp &&t_col)
	: GameObject{ p_sprite, std::move(t_col) }, hp{ HP },
	  direction{ RIGHT }, vulnerable {true}
{
}

void Characters::Set_HP(int hit_points)
{
	hp = hit_points;
}

void Characters::Decrease_HP(const char damage) 
{
	if (vulnerable)
	{
		hp -= damage;
		//std::cout << "HP reduced! " << hp << std::endl;
	}
}

int Characters::Get_HP() const
{
	return hp;
}

void Characters::Increase_HP(const int increment)
{
	hp += increment;
}

Direction Characters::Get_Direction(void) const
{
	return direction;
}

void Characters::Set_Direction(Direction d)
{
	direction = d;
}

void Characters::Set_Vulnerable(const bool status)
{
	vulnerable = status;
}

bool Characters::Get_Vulnerable() const
{
	return vulnerable;
}

int Characters::Get_Score()
{
	return score;
}

void Characters::Add_Score(const int s)
{
	score += s;
}

void Characters::Reset_Score()
{
	score = 0;
}

float Characters::Get_Idle_Time() const
{
	return idle_timing;
}

void Characters::Reset_Idle_Time(float time)
{
	idle_timing = time;
}

void Characters::Decrease_Idle_Time(float time)
{
	idle_timing -= time;
}


int Characters::Get_Enemies_Killed()
{
	return enemies_killed;
}
void Characters::Add_Kill_count()
{
	++enemies_killed;
}
void Characters::Reset_Enemy_Killed()
{
	enemies_killed = 0;
}