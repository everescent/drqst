/* Start Header ************************************************************************/
/*!
\file      Boss_States.h
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
#pragma once
#include <utility>
#include "Projectile.h"


// States for boss actions
enum Boss_Action_State
{
	IDLE = 0,
	MOVING,
	ATTACK,
	OBSTACLE,
	DEAD
};




/**************************************************************************************
//
// A type to store ai attack and the cooldown
//
**************************************************************************************/
struct Boss_Attack : public Projectile
{
	float cooldown_timer;                   // cooldown duration
	bool cooldown, ongoing_attack = false;  // bool to check if if a skill is on cooldown or an attack is ongoing

	// default constructor
	Boss_Attack()
		: cooldown_timer{ 0 }, cooldown{ false },
		  Projectile()
	{
	}

	// converting constructor
	Boss_Attack(Sprite* t_attack, Col_Comp &&t_col)
		:cooldown_timer{0}, Projectile(t_attack, std::move(t_col)), 
		 cooldown{false}
	{
	}

	void Update(float dt);                            // updates the attacks
	void Start_Attack(const float x, const float y);  // change the variables to start an attack
	void End_Attack();								  // change variables back to its default state
};

//returns a random number within a range
int Get_Random_Num(int range);