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

#include "Characters.h" // header file
#include <iostream>
#include <utility>

 int Characters::score = 0;            // player score for the stage
 int Characters::enemies_killed = 0;   // number of enemies killed

/**************************************************************************************
//
// Converting constructor for characters
//
**************************************************************************************/
Characters::Characters(Sprite* p_sprite, const int HP, Col_Comp &&t_col)
	: GameObject{ p_sprite, std::move(t_col)  }, hp{ HP },
	  direction { RIGHT },  vulnerable { true }
{
}
/**************************************************************************************
//
// Setter for character HP
//
**************************************************************************************/
void Characters::Set_HP(int hit_points)
{
	hp = hit_points;
}
/**************************************************************************************
//
// Decrease character hp if they are vulnerable by the damage given
//
**************************************************************************************/
void Characters::Decrease_HP(const char damage) 
{
	if (vulnerable)
	{
		hp -= damage;
		//std::cout << "HP reduced! " << hp << std::endl;
	}
}
/**************************************************************************************
//
// Getter for character HP
//
**************************************************************************************/
int Characters::Get_HP() const
{
	return hp;
}
/**************************************************************************************
//
// increase character hp by the increment given
//
**************************************************************************************/
void Characters::Increase_HP(const int increment)
{
	hp += increment;
}
/**************************************************************************************
//
// Getter for character direction
//
**************************************************************************************/
Direction Characters::Get_Direction(void) const
{
	return direction;
}
/**************************************************************************************
//
// Setter for character direction
//
**************************************************************************************/
void Characters::Set_Direction(Direction d)
{
	direction = d;
}
/**************************************************************************************
//
// Setter for character vulnerability state
//
**************************************************************************************/
void Characters::Set_Vulnerable(const bool status)
{
	vulnerable = status;
}
/**************************************************************************************
//
// Getter for character vulnerability state
//
**************************************************************************************/
bool Characters::Get_Vulnerable() const
{
	return vulnerable;
}
/**************************************************************************************
//
// Getter for current score
//
**************************************************************************************/
int Characters::Get_Score()
{
	return score;
}
/**************************************************************************************
//
// adds a given score to current score
//
**************************************************************************************/
void Characters::Add_Score(const int s)
{
	score += s;
}
/**************************************************************************************
//
// Resets the score to 0
//
**************************************************************************************/
void Characters::Reset_Score()
{
	score = 0;
}
/**************************************************************************************
//
// Getter for idling time
//
**************************************************************************************/
float Characters::Get_Idle_Time() const
{
	return idle_timing;
}
/**************************************************************************************
//
// Setter for idling time
//
**************************************************************************************/
void Characters::Reset_Idle_Time(float time)
{
	idle_timing = time;
}
/**************************************************************************************
//
// Decrease idling time by the given amount
//
**************************************************************************************/
void Characters::Decrease_Idle_Time(float time)
{
	idle_timing -= time;
}
/**************************************************************************************
//
// Getter for enemies killed
//
**************************************************************************************/
int Characters::Get_Enemies_Killed()
{
	return enemies_killed;
}
/**************************************************************************************
//
// Add 1 to kill count
//
**************************************************************************************/
void Characters::Add_Kill_count()
{
	++enemies_killed;
}
/**************************************************************************************
//
// Resets kill count to 0
//
**************************************************************************************/
void Characters::Reset_Enemy_Killed()
{
	enemies_killed = 0;
}