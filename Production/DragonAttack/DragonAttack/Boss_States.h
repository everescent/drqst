/* Start Header ************************************************************************/
/*!
\file      Boss_States.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for Boss_States

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "Projectile.h" // parent class of boss attack
#include <utility>      // std::move


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