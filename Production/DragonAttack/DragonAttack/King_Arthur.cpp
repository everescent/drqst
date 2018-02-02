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
#include <iostream>
#include "King_Arthur.h"
#include "Boss_States.h"
#include <vector>

namespace {

	void King_Arthur_Phase2(void); // changes mechanics for phase 2

	void Jump_Attack(); // jump attack

	void Single_Slash(); // single slash + wave

	void Triple_Slash(); // single slash 3 times

	void Heal_and_Spawn(); // phase 2, heal and spawm mobs

	std::vector <Boss_Attack> arthur; //an array of boss attacks

	Boss_Action_State current_action = ATTACK; // different states of boss arthur

	const int HP = 3000; // initial hp for king arthur

	const int phase2_hp = 1500; //phase 2 trigger

	Col_Comp t_col{ 0.0f, 0.0f, 5.0f, 5.0f, Rect}; // ??

}



King_Arthur::King_Arthur(Sprite&& t_sprite)
	: Characters(std::move(t_sprite), HP, std::move(t_col)), phase2{false}
{
	/* const float x, y  // to place the position of king arthur
	   scale and translate king arthur's position*/
	this->Transform_.SetTranslate(100.0f, 100.0f);

	Init();
}

void King_Arthur::Init(void)
{
	const char* tex_ss = ".//Textures/1.jpg";
	const char* tex_3s = ".//Textures/holylight.jpg";
	const char* tex_ja = ".//Textures/light_texture2320.jpg";

	arthur.push_back(Boss_Attack(S_CreateSquare(20.0f, 1.0f, 1.0f, tex_ss),
					 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	arthur.push_back(Boss_Attack(S_CreateSquare(10.0f, 1.0f, 1.0f, tex_3s),
					 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	arthur.push_back(Boss_Attack(S_CreateSquare(30.0f, 1.0f, 1.0f, tex_ja),
					 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	(void)arthur[0].SetVelocity(AEVec2{ 1.0f, 0.0f });
	(void)arthur[1].SetVelocity(AEVec2{ 1.0f, 0.0f });
}

void King_Arthur::Update(float dt) 
{

	if (this->Get_HP() < phase2_hp)
	{
		King_Arthur_Phase2();
	}

	switch (current_action)
	{
	case IDLE: this->Idle(dt);
		break;

	case MOVING: this->Moving();
		break;

	case OBSTACLE: this->AvoidingObstacle();
		break;

	case ATTACK: this->Attack(dt);
		break;
	}


// use the dt to update the cooldown of the attacks?
// king arthur has to change between idling, attacking and moving
// nede to change the direction arthur is facing when player is behind
}

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
	UNREFERENCED_PARAMETER(dt);
	static float idle = 0.0f; //determine how long king arthurs stops

	if (!idle) //sets the timer to 3 seconds 
	{
		idle = 3.0f;
		return;
	}
	
	idle <= 0? current_action = ATTACK, idle = 0 : idle -= 0.016f;
	
}

void King_Arthur::Moving(void)
{
	//move, check if it is nearing the player, attacks the player 
	// or check if jump attack is available
	// during phase 2, check if player is on a platform

	//moves towards player for 2 seconds~

	//changes BOTH direction of attacks and king arthur to face player
	if(false)
	for (int i = 0; i < 3; ++i)
	{
		arthur[i].SetDir(true);
	}
}

void King_Arthur::Attack(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	//randomize an attack to be used within the 3 attacks
	//call the corresponding function to execute it

	
	Single_Slash();

	for (int i = 0; i < 3; ++i)
		arthur[i].Update(0.16f);

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
		// play some cinematic shit

		arthur.pop_back();

		const char* tex_ja = ".//Textures/light_texture2320.jpg";
		arthur.push_back(Boss_Attack(S_CreateSquare(30.0f, 1.0f, 1.0f, tex_ja), 
						 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	}

	void Jump_Attack(void)
	{
		//calculate the physics to jump and attack player
	}

	void Single_Slash(void)
	{
		//do a single slash at the player
		arthur[0].SetActive(true);

		arthur[0].cooldown_timer = 2.0f; // start the cooldown of the skill

		if (arthur[0].GetDist() > 50)
		{
			arthur[0].SetActive(false);
			arthur[0].ResetDist();
			arthur[0].PosX = 0.0f;
			current_action = IDLE;

		}

		arthur[0].Pos();
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


King_Arthur* Spawn_KA(void)
{

	const char* tex_ka = ".//Textures/download.jpg";

	King_Arthur *ka = new King_Arthur(S_CreateSquare(100.0f, 1.0f, 1.0f, tex_ka));

	return ka;


}

void Free_KA(King_Arthur *ka)
{
	delete ka;
}