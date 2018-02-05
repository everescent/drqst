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
#include "Dragon.h"
#include <vector>
// @TODO, KA behaviour, physics for jump attack
namespace {

	void King_Arthur_Phase2(void); // changes mechanics for phase 2

	void Jump_Attack(); // jump attack

	void Single_Slash(void); // single slash + wave

	void Triple_Slash(); // single slash 3 times

	void Heal_and_Spawn(); // phase 2, heal and spawm mobs

	std::vector <Boss_Attack> arthur; //an array of boss attacks

	Boss_Action_State current_action = MOVING; // different states of boss arthur

	const int health = 3000; // initial hp for king arthur

	const int phase2_hp = 1500; //phase 2 trigger

	const int interval = 50; // interval for triple slash

	const int range_limit = 300; // range limit for slash

	const char limit = 5; // num of king arthur attacks

	bool phase2 = false;

	enum KA_MoveSet // for switch case to determine which attack gets called
	{
		SINGLE_SLASH = 1,
		TRIPLE_SLASH,
		UNIQUE_MECHANIC
	};
	
}



King_Arthur::King_Arthur()
	: Characters(S_CreateSquare(100.0f, 1.0f, 1.0f, ".//Textures/download.png"),
	  health, { 0.0f, 0.0f, 5.0f, 5.0f, Rect } ), phase2{false}
{
	(void) this->Transform_.SetTranslate(200.0f, -100.0f);
	this->Transform_.Concat(); // spawn king arthur at the location set
	this->PosX =  200.0f;      // change king arthur coordinates to the location set
	this->PosY = Start_Pos_Y;  // change king arthur coordinates to the location set
	this->SetActive(true);     // show him on screen
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	Init();  // call initializer for king arthur move set
}

void King_Arthur::Init(void)
{
	const char* tex_ss = ".//Textures/1.jpg"; 
	const char* tex_3s = ".//Textures/holylight.jpg";
	const char* tex_ja = ".//Textures/light_texture2320.jpg";

	arthur.reserve(3);

	arthur.emplace_back(Boss_Attack(S_CreateSquare(20.0f, 1.0f, 1.0f, tex_ss), // for single slash
					 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); 

	arthur.emplace_back(Boss_Attack(S_CreateSquare(10.0f, 1.0f, 1.0f, tex_3s), // 1st triple slash 
					 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	arthur.emplace_back(Boss_Attack(S_CreateSquare(10.0f, 1.0f, 1.0f, tex_3s), // 2nd single slash
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	arthur.emplace_back(Boss_Attack(S_CreateSquare(10.0f, 1.0f, 1.0f, tex_3s), // 3rd triple slash
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	arthur.emplace_back(Boss_Attack(S_CreateSquare(30.0f, 1.0f, 1.0f, tex_ja), // jump attack
					 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	(void)arthur[0].SetVelocity(AEVec2{ 3.0f, 0.0f }); // velocity of the slashes
	(void)arthur[1].SetVelocity(AEVec2{ 3.0f, 0.0f }); // velocity of the slashes
	(void)arthur[2].SetVelocity(AEVec2{ 3.0f, 0.0f }); // velocity of the slashes
	(void)arthur[3].SetVelocity(AEVec2{ 3.0f, 0.0f }); // velocity of the slashes

	arthur[0].Transform_.SetScale(2.0f, 2.0f); // determine the size of projectile
	arthur[0].Transform_.Concat();

	for (char i = 0; i < limit; ++i) // spawning location of slashes
		arthur[i].PosY = -120.0f;

}

void King_Arthur::Update(float dt, const Dragon &d) 
{

	if (this->Get_HP() < phase2_hp) // activate phase 2 once hp drops is 50%
	{
		King_Arthur_Phase2();
	}

	// switch between the boss states
	switch (current_action)
	{
	case IDLE: this->Idle(dt);
		break;

	case MOVING: this->Moving(d,dt);
		break;

	case OBSTACLE: this->AvoidingObstacle();
		break;

	case ATTACK: this->Attack(dt);
		break;
	}

	for (int i = 0; i < 4; ++i) // update cooldowns on attacks
		if(arthur[i].cooldown)
		arthur[i].Update(0.016f);

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
	static float idle = 3.0f; //determine how long king arthurs stops

	if (!idle) //sets the timer to 3 seconds 
	{
		idle = 3.0f;
	}
	
	//changes state to attack once idling is finished, reset idle
	idle <= 0? current_action = ATTACK, idle = 0 : idle -= 0.016f;
	
}

void King_Arthur::Moving(const Dragon &d, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	
	//move, check if it is nearing the player, attacks the player 
	// or check if jump attack is available
	// during phase 2, check if player is on a platform

	//moves towards player for 2 seconds~

	//changes BOTH direction of attacks and king arthur to face player
	
	static float move_duration = 1.5f; // duration KA moves
	Direction dirLR = LEFT; //determines if he goes left or right
	
	if (!move_duration) //set the timer for moving
		move_duration = 1.5f;
	
	if (d.PosX - this->PosX == 0) // character and boss intersect
		dirLR = STAY;
	else
	//determine if dragon is right or left of king arthur
	dirLR = (d.PosX - this->PosX) > 0 ? RIGHT : LEFT; 

	/*if(phase2)
		d.PosY - this->PosY;*/
	
	
	if (dirLR == RIGHT) // set all attacks to go right
	{
		for (int i = 0; i < limit; ++i)
		{
			arthur[i].SetDir(true);
		}

		this->PosX += 2.0f; // move KA to the right
		(void) this->Transform_.SetTranslate(PosX, PosY);
	}
	else if (dirLR == LEFT) // set all attacks to go left
	{
		for (int i = 0; i < limit; ++i)
		{
			arthur[i].SetDir(false);
		}
		this->PosX -= 2.0f; // move KA to the left
		(void) this->Transform_.SetTranslate(PosX, PosY);
	}

	this->Transform_.Concat();

	//change state to attack once move_duration is 0, reset move_duration
	move_duration <= 0 ? current_action = ATTACK, move_duration = 0 :
		                 move_duration -= 0.016f;

}

void King_Arthur::Attack(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	//randomize an attack to be used within the 3 attacks
	//call the corresponding function to execute it
	
	KA_MoveSet currAttk = SINGLE_SLASH;
	
	/*if (! (arthur[4].cooldown) )
		currAttk = UNIQUE_MECHANIC;

	else*/ 
	if (!(arthur[1].cooldown)) //problem lies in setting to cooldown to true
		currAttk = TRIPLE_SLASH;

	switch (currAttk) // calls the attack move chosen
	{
	case SINGLE_SLASH: 
		if (!(arthur[0].cooldown_timer)) // sets the attack to start from KA
		{
			arthur[0].PosX = this->PosX;
			arthur[0].SetActive(true);
		}

		Single_Slash();
		break;

	case TRIPLE_SLASH: 
		if (!(arthur[1].cooldown_timer)) // sets the attack to start from KA
		{
			for (char i = 1; i < limit - 1; ++i)
			{
				arthur[i].SetActive(true);
				arthur[i].PosX = this->PosX;
			}
		}

		Triple_Slash();
		break;

	case UNIQUE_MECHANIC: Single_Slash();
		break;

	default:
		break;
	
	}

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

		arthur.pop_back(); // delete jump attacl

		const char* tex_ja = ".//Textures/light_texture2320.jpg";
		arthur.emplace_back(Boss_Attack(S_CreateSquare(30.0f, 1.0f, 1.0f, tex_ja), 
						 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); // add the 2nd mechanic, still in progress

		phase2 = true;
	}

	void Jump_Attack(void)
	{
		//calculate the physics to jump and attack player
		// calculate the distance between KA and player current distance?
		// do a jump for that much distance?

		//bool flag = false; // for jump in progress
		//static float distance_to_jump = 0.0f; // store distance to jump

	}

	void Single_Slash(void) 
	{
		if (arthur[0].GetDist() > range_limit) //disappears once it flys 500 unit away
		{
			arthur[0].SetActive(false); // makes it disappears form screen

			arthur[0].ResetDist(); // error here, need to reset it to king arthur's position			

			current_action = IDLE; // set the behaviour to idle

			arthur[0].PosX = 0.0f;  // to remove flicker

			arthur[0].cooldown = true; // skill on cooldown

		}
		
		arthur[0].Projectile::Pos(); // need to check what the rotation does
		arthur[0].Projectile::Update(); // check what the sqrt does also
		arthur[0].Render();

		arthur[0].cooldown_timer = 2.0f;
	}

	void Triple_Slash(void)
	{
		char ts_limit = limit - 1; // limit the loop to just the triple slashes

		if (arthur[1].cooldown) // skill still on cooldown
			return;

		//slashes 3 times at the player with an interval inbetween
		for (char i = 1; i < ts_limit; ++i)
		{
			if (arthur[i].GetDist() > range_limit) // makes projectiles disappear
			{
				arthur[i].SetActive(false); // disappears from screen
				arthur[i].ResetDist();     // reset the projectile distance traveled
				arthur[i].PosX = 0.0f; // remove flicker

				if (i == 3) // last slash has completed
				{
					current_action = IDLE; // change state to idle
					arthur[1].cooldown = true; // start cooldown		
				}
			}
		}	
		// issue here if i want to use jacob projectile update and pos
		if (arthur[1].GetDist() > interval || ! (arthur[1].IsActive() ) )
		{
			//fire the second projectile once the first 1 travels 50 pixels
			arthur[2].Pos();

			if (arthur[2].GetDist() > interval || ! (arthur[2].IsActive() ) )
				// fires the third one once the second 1 travels 0 pixel
				arthur[3].Pos();
		}

		arthur[1].Pos(); // render the first slash
		arthur[1].cooldown_timer = 10.0f;

		
	}

	void Heal_and_Spawn(void)
	{
		//teleport to a platform, heal and spawn mobs
	}
}


