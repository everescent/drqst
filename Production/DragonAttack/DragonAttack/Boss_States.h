/* Start Header ************************************************************************/
/*!
\file       AI_Data_Factory.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file has the ai behaviour enums and a struct to create projectile attacks

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



// A type to store boss attack and the cooldown
struct Boss_Attack : public Projectile
{
	float cooldown_timer;
	bool cooldown, ongoing_attack = false;

	Boss_Attack()
		: cooldown_timer{ 0 }, cooldown{ false },
		  Projectile()
	{
	}

	Boss_Attack(const Sprite* t_attack, Col_Comp &&t_col)
		:cooldown_timer{0}, Projectile(t_attack, std::move(t_col)), 
		 cooldown{false}
	{
	}

	void Update() {}; // ignored
	void Update(float dt);  //has to check cooldown_timer
	void Start_Attack(const float x, const float y);
	void End_Attack();
	//void Pos(); 

};

//returns a random number within a range
int Get_Random_Num(int range);
void seed_initializer(void);