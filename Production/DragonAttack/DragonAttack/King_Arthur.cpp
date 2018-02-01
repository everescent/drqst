/* Start Header ************************************************************************/
/*!
\file       King_Arthur.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the last boss king arthur

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "King_Arthur.h"
#include "Boss_States.h"

namespace {

	void King_Arthur_Phase2(void); // changes mechanics for phase 2

	void Jump_Attack(); // jump attack

	void Single_Slash(); // single slash + wave

	void Triple_Slash(); // single slash 3 times

	void Heal_and_Spawn(); // phase 2, heal and spawm mobs

	//Boss_Attack arthur[3];  need to use constructors to fill up the attacks

	Boss_Action_State current_action = IDLE;

	const int HP = 3000;

	Col_Comp  t_col{ 0.0f, 0.0f, 5.0f, 5.0f, Rect};

}



King_Arthur::King_Arthur(Sprite&& t_sprite)
	: Characters(std::move(t_sprite), HP, t_col), phase2{false}
{
	/* const float x, y  // to place the position of king arthur
	   scale and translate king arthur's position*/
}

//void King_Arthur::Update(float dt)
//{
//	switch (current_action)
//	{
//	case IDLE: 
//		break;
//
//	case MOVING: 
//		break;
//
//	case OBSTACLE:
//		break;
//
//	case ATTACK:
//		break;
//	}

// use the dt to update the cooldown of the attacks?
// king arthur has to change between idling, attacking and moving
// nede to change the direction arthur is facing when player is behind
//}

void King_Arthur::Pos()
{

}
/******************************************************************************/
/*!
\fn         Idle

\brief      King arthur pauses for awhile for the player to refocus

\param      float dt
- delta time to calculate how long king arthur freezes

\retval     void
No return.
*/
/******************************************************************************/
void King_Arthur::Idle(float dt)
{
	static float idle = 0.0f; //determine how long king arthurs stops

	if (!idle) //sets the timer to 3 seconds 
	{
		idle = 3.0f;
		return;
	}
	
	idle <= 0? current_action = MOVING, idle = 0 : idle -= dt;
	
}

void King_Arthur::Moving(void)
{
	//move, check if it is nearing the player, attacks the player 
	// or check if jump attack is available
	// during phase 2, check if player is on a platform

	//moves towards player for 2 seconds~

}

void King_Arthur::Attack(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	//randomize an attack to be used within the 3 attacks
	//call the corresponding function to execute it

}

void King_Arthur::AvoidingObstacle(void)
{
	//move to player in phase 2 with platforms
	//path finding i guess?
}
namespace {

	void King_Arthur_Phase2(void)
	{
		// change the phase 1 machanic to phase 2

	}

	void Jump_Attack(void)
	{
		//calculate the physics to jump and attack player
	}

	void Single_Slash(void)
	{
		//do a single slash at the player
	}

	void Triple_Slash(void)
	{
		//slashes 3 times at the player with an interval inbetween
	}

	void Heal_and_Spawn(void)
	{
		//teleport to a platform, heal and spawn mobs
	}
}