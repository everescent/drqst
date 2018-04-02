/* Start Header ************************************************************************/
/*!
\file      Boss_States.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file has the ai behaviour enums and a struct for their attacks

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Boss_States.h"
#include <cstdlib>
#include <ctime>

/**************************************************************************************
//
// update the cooldowns of the attacks
//
**************************************************************************************/
void Boss_Attack::Update(float dt) 
{
	// attack is on cooldown
	if (cooldown)
	{
		// reduce timer by dt
		cooldown_timer -= dt;

		if (cooldown_timer <= 0)
		{
			cooldown_timer = 0; // set cooldown timer to 0
			cooldown = false;	// attack is no longer on cooldown
		}
	}
}
/**************************************************************************************
//
// update attacks to the position given
//
**************************************************************************************/
void Boss_Attack::Start_Attack(const float x, const float y)
{
	PosX = x;                // start x position of attack
	PosY = y;                // satrt y position of attack
	SetActive(true);         // render attack on screen
	ongoing_attack = true;   // attack is ongoing
}
/**************************************************************************************
//
// Reset the variables in attack for future usage
//
**************************************************************************************/
void Boss_Attack::End_Attack()
{
	SetActive(false);       // make stab disappaer
	cooldown = true;        // start cooldown
	ResetDist();            // reset distance traveled back to 0
	SetCollided(false);     // reset collided flag
	ongoing_attack = false; // attack animation has concluded
}
/**************************************************************************************
//
// generate a random number from a given range
//
**************************************************************************************/
int Get_Random_Num(int range)
{
	return rand() % (range + 1);
}
