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

	std::vector <Boss_Attack> arthur; //an array of boss attacks

	std::vector <Characters> spawn_mobs; //an array to store the mobs to be spawn

	Boss_Action_State current_action = IDLE; // different states of boss arthur

	const int health = 300; // initial hp for king arthur

	const int phase2_hp = 1500; //phase 2 trigger

	const int interval = 80; // interval for triple slash

	const int range_limit = 500; // range limit for slash

	const float start_point_x = 200.0f;

	const float start_point_y = -150.0f;

	const float slash_box = 40.0f;

	const char limit = 5; // num of king arthur attacks

	char behavior_swap = 0; // switch between idling and moving

	void Move_KA(const float dt, King_Arthur &ka, const Dragon &d); // move king arthur towards player

	void Set_Attack_Dir(King_Arthur &ka); // set the attack directions
	
}



King_Arthur::King_Arthur(void)
	: Characters(S_CreateSquare(100.0f, ".//Textures/download.png"), health,
	  Col_Comp{ start_point_x - 30.0f, start_point_y - 30.0f,
				start_point_x + 30.0f, start_point_y + 30.0f, Rect } ), 
	phase2{false}
{
	this->PosX =  start_point_x;         // change king arthur coordinates to the location set
	this->PosY =  start_point_y;         // change king arthur coordinates to the location set
	this->SetActive(true);               // show him on screen
	this->Set_Direction(LEFT);           // set king arthur to face left at the start
	this->SetVelocity({ 120, 0 });       // velocity for king arthur
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	(void) this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();           // spawn king arthur at the location set
	Init();							     // call initializer for king arthur move set
}

void King_Arthur::Init(void)
{
	const char* tex_ss = ".//Textures/1.jpg"; 
	const char* tex_3s = ".//Textures/holylight.jpg";
	const char* tex_ja = ".//Textures/light_texture2320.jpg";

	arthur.reserve(limit);

	arthur.emplace_back(Boss_Attack(S_CreateSquare(20.0, tex_ss), // for single slash
					 Col_Comp(start_point_x - slash_box, start_point_y -  slash_box,
							  start_point_y + slash_box, start_point_y + slash_box, Rect)));

	arthur.emplace_back(Boss_Attack(S_CreateSquare(20.0, tex_3s), // 1st triple slash 
		Col_Comp(start_point_x - slash_box, start_point_y - slash_box,
				 start_point_y + slash_box, start_point_y + slash_box, Rect)));

	arthur.emplace_back(Boss_Attack(S_CreateSquare(20.0, tex_3s), // 2nd single slash
		Col_Comp(start_point_x - slash_box, start_point_y - slash_box,
				 start_point_y + slash_box, start_point_y + slash_box, Rect)));

	arthur.emplace_back(Boss_Attack(S_CreateSquare(20.0, tex_3s), // 3rd triple slash
		Col_Comp(start_point_x - slash_box, start_point_y - slash_box,
				 start_point_y + slash_box, start_point_y + slash_box, Rect)));

	arthur.emplace_back(Boss_Attack(S_CreateSquare(slash_box, tex_ja), // jump attack
					 Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	for (char i = 0; i < limit - 1; ++i) // initializing other variables in slash
	{
		arthur[i].PosY = start_point_y;
(void)  arthur[i].SetVelocity(AEVec2{ 200.0f, 0.0f }); // velocity for slash
(void)  arthur[i].Transform_.SetScale(1.5f, 1.5f); // determine the size of projectile
(void)	arthur[i].Transform_.SetTranslate( start_point_x, start_point_y );
		arthur[i].Transform_.Concat();
	}

	ka_attacks[SINGLE_SLASH]    = &King_Arthur::Single_Slash;
	ka_attacks[TRIPLE_SLASH]    = &King_Arthur::Triple_Slash;
	ka_attacks[UNIQUE_MECHANIC] = &King_Arthur::Dash_Attack;

}

void King_Arthur::Update(const float dt, Dragon &d) 
{
	(d.PosX - this->PosX) > 0 ? this->Set_Direction(RIGHT) :
	this->Set_Direction(LEFT);
	
	//if (this->Get_HP() < phase2_hp) // activate phase 2 once hp drops is 50%
	//{
	//	King_Arthur_Phase2();
	//}

	if (behavior_swap == 3)
	{
		current_action = MOVING;  // switch state to moving
		behavior_swap = 0;        // reset the counter
	}

	for (char i = 0; i < Bullet_Buffer; ++i)
		if (d.GetFireball()[i].IsActive())
			if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, this->GetVelocity(), d.GetFireball()[i].GetVelocity(), dt))
			{
				Decrease_HP(d.GetDamage());
				d.GetFireball()[i].SetActive(false);
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

	case ATTACK: this->Attack(d, dt);
		break;

	default: break;
	}

	for (char i = 0; i < limit; ++i) // update cooldowns on attacks
			arthur[i].Update(dt);


	
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
void King_Arthur::Idle(const float dt)
{
	UNREFERENCED_PARAMETER(dt);
	static float idle = 2.0f; //determine how long king arthurs stops

	if (!idle) //sets the timer to 3 seconds 
	{
		idle = 2.0f;
	}
	
	//changes state to attack once idling is finished, reset idle
	idle <= 0? current_action = ATTACK, idle = 0 : idle -= dt;
	Set_Attack_Dir(*this);
	
}

void King_Arthur::Moving(const Dragon &d, const float dt)
{	
	static float move_duration = 3.0f; // duration KA moves
	
	if (!move_duration) //set the timer for moving
		move_duration = 3.0f;
	
	if( (int) (d.PosX - this->PosX) == 0) // character and boss intersect
		this->Set_Direction(STAY);
	else
	//determine if dragon is right or left of king arthur
	(d.PosX - this->PosX) > 0 ? this->Set_Direction(RIGHT):
	this->Set_Direction(LEFT);

	/*if(phase2)
		d.PosY - this->PosY;*/
	
	Move_KA(dt, *this, d);

	//change state to attack once move_duration is 0, reset move_duration
	move_duration <= 0 || this->Get_Direction() == STAY ? 
    current_action = ATTACK, move_duration = 0 : move_duration -= dt;

	Set_Attack_Dir(*this);
}

void King_Arthur::Attack(Dragon &d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(d);

	static KA_MoveSet currAttk;
	
	currAttk = SINGLE_SLASH; //default attack for king arthur

	//unique mechanic has the highest priority 
	if (! arthur[currAttk].ongoing_attack)
	{
		if (!(arthur[4].cooldown))
			currAttk = UNIQUE_MECHANIC;

		//followed by triple slash
		/*else if (!(arthur[1].cooldown))
			currAttk = TRIPLE_SLASH;*/
	}

	(this->*ka_attacks[currAttk])(d, dt); // call the coresponding attack function
}

void King_Arthur::AvoidingObstacle(void)
{
	//move to player in phase 2 with platforms
	//path finding i guess?
}

void King_Arthur::King_Arthur_Phase2(void)
{
	// change the phase 1 machanic to phase 2
	// play some cinematic shit
	// healing should be just particle effects

	arthur.pop_back(); // delete jump attack

	const char* tex_ja = ".//Textures/light_texture2320.jpg";
	arthur.emplace_back(Boss_Attack(S_CreateSquare(30.0f, tex_ja),
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); // add the 2nd mechanic, still in progress

	this->phase2 = true; // set phase 2 flag to true

	// change unique mechanism pointer to heal and spawn for phase 2
	ka_attacks[UNIQUE_MECHANIC] = &King_Arthur::Heal_and_Spawn; 
}

void King_Arthur::Dash_Attack(Dragon &d, const float dt)
{	
	if (arthur[4].cooldown) // skill still on cooldown
		return;

	static Direction dash;           // direction KA should dash in

	if (!(arthur[4].cooldown_timer)) // called once to fix dash direction
	{
		dash = this->Get_Direction();
		arthur[2].ongoing_attack = true;
	}

	this->Set_Direction(dash);       // change back the dash direction

	const short left_boundary = -600; // boundaries of charge attack
    const short right_boundary = 600; // boundaries of charge attack

	this->SetVelocity({ 800, 0 }); // change king arthur's velocity to dash
	Move_KA(dt, *this, d);         // move king arthur towards player

	// need check for collision against dragon
	if(this->PosX > right_boundary || this->PosX < left_boundary ||
	   this->Get_Direction() == STAY)
	{
		current_action = IDLE;           // switch current state to idle
		arthur[4].cooldown = true;       // start the cooldown of dash attk
		this->SetVelocity({ 120, 0 });   // reset the velocity of king arthur
		++behavior_swap;    
		arthur[2].ongoing_attack = false;

		if(this->Get_Direction() == STAY)
			d.Decrease_HP();
	}

	this->Transform_.Concat();

	arthur[4].cooldown_timer = 10.0f; // cooldown of jump attack
}

void King_Arthur::Single_Slash(Dragon &d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);
	
	if (arthur[0].cooldown) // skill still on cooldown
		return;

	if (!(arthur[0].IsActive())) // sets the attack to start from KA
	{
		arthur[0].PosX = this->PosX;
		arthur[0].SetActive(true);
		arthur[0].ongoing_attack = true;
	}

	bool collided = arthur[0].Collision_.Dy_Rect_Rect(d.Collision_, arthur[0].GetVelocity(), d.GetVelocity(), dt);
	
	//disappears once it flys 500 unit away
	if (arthur[0].GetDist() > range_limit || collided) 
	{
		arthur[0].SetActive(false); // makes it disappears form screen

		arthur[0].ResetDist(); 		// reset to 0 distance traveled

		current_action = IDLE; // set the behaviour to idle

		arthur[0].PosX = 0.0f;  // to remove flicker

		arthur[0].cooldown = true; // skill on cooldown

		arthur[0].ongoing_attack = false;

		if (collided)
		{
			d.Decrease_HP();
		}

		++behavior_swap;
	}
	
	arthur[0].Projectile::Pos();    // need to check what the rotation does
	arthur[0].Projectile::Update(); // check what the sqrt does also
	arthur[0].Collision_.Update_Col_Pos(arthur[0].PosX - slash_box, arthur[0].PosY - slash_box,
										arthur[0].PosX + slash_box, arthur[0].PosY + slash_box);
	

	arthur[0].cooldown_timer = 2.0f;
}

void King_Arthur::Triple_Slash(Dragon &d, const float dt)
{
	
	UNREFERENCED_PARAMETER(d);
	UNREFERENCED_PARAMETER(dt);

	bool collided[3]  { 0 };

	if (arthur[1].cooldown) // skill still on cooldown
		return;

	static bool sec_flag = true, third_flag = true; // for second and third wave
	static bool hit = false;						// check if slash hit the player

	const char ts_limit = limit - 1; // limit the loop to just the triple slashes
	
	if (!(arthur[1].cooldown_timer)) // sets the attack to start from KA
	{
		for (char i = 1; i < ts_limit; ++i)
		{
			arthur[i].PosX = this->PosX;
		}
		arthur[1].SetActive(true);         // set the first attack to show
		arthur[1].ongoing_attack = true;   // attack is currently ongoing
	}

	if (arthur[1].GetDist() > interval || ! (arthur[1].IsActive() ) )
	{
		//fire the second projectile once the first 1 travels 50 pixels

		if (sec_flag)
		{
			arthur[2].SetActive(true);
			sec_flag = false;
		}

		arthur[2].Projectile::Pos();
		arthur[2].Projectile::Update();
		arthur[2].cooldown_timer = 5.0f;

		arthur[2].Collision_.Update_Col_Pos(arthur[2].PosX - slash_box, arthur[2].PosY - slash_box,
											arthur[2].PosX + slash_box, arthur[2].PosY + slash_box);

		if (arthur[2].GetDist() > interval || !(arthur[2].IsActive()))
		{	// fires the third one once the second 1 travels 0 pixel

			if (third_flag)
			{
				arthur[3].SetActive(true);
				third_flag = false;
			}
			arthur[3].Projectile::Pos();
			arthur[3].Projectile::Update();
			arthur[3].cooldown_timer = 5.0f;

			arthur[3].Collision_.Update_Col_Pos(arthur[3].PosX - slash_box, arthur[3].PosY - slash_box,
												arthur[3].PosX + slash_box, arthur[3].PosY + slash_box);
		}
	}

	if (! hit)
	{
		for (int i = 0; i < 3; ++i)
		{
			collided[i] = arthur[i].Collision_.Dy_Rect_Rect(d.Collision_, arthur[i].GetVelocity(), d.GetVelocity(), dt);

			if (collided[i])
			{
				d.Decrease_HP();
				hit = true;
				break;
			}
		}
	}

	//slashes 3 times at the player with an interval inbetween
	for (char i = 1; i < ts_limit; ++i)
	{
		if (arthur[i].GetDist() > range_limit) // makes projectiles disappear
		{
			arthur[i].SetActive(false); // disappears from screen
			arthur[i].ResetDist();      // reset the projectile distance traveled
			arthur[i].PosX = 0.0f;      // remove flicker

			if (i == 3) // last slash has completed
			{
				current_action = IDLE;     // change state to idle
				arthur[1].cooldown = true; // start cooldown		
				++behavior_swap;
				sec_flag = third_flag = true;
				arthur[1].ongoing_attack = false;
				hit = false;
			}
		}
	}


	arthur[1].Projectile::Pos(); // render the first slash
	arthur[1].Projectile::Update();
	arthur[1].Collision_.Update_Col_Pos(arthur[1].PosX - slash_box, arthur[1].PosY - slash_box,
										arthur[1].PosX + slash_box, arthur[1].PosY + slash_box);


	arthur[1].cooldown_timer = 5.0f;
}

void King_Arthur::Heal_and_Spawn(Dragon &d, const float dt)
{
	UNREFERENCED_PARAMETER(d);
	UNREFERENCED_PARAMETER(dt);
	//teleport to a platform, heal and spawn mobs

	if (!(arthur[4].cooldown_timer)) // teleport once
	{
		//get a random number and teleport to the coresponding platform
		switch (Get_Random_Num(3)) 
		{
		case 1: this->PosX = 300.0f, this->PosY = 100.0f;
			break;

		case 2: this->PosX = -300.0f, this->PosY = 100.0f;
			break;

		case 3: this->PosX = 50.0f, this->PosY = 200.0f;
			break;

		default: break;
		}
		this->Transform_.SetTranslate(this->PosX, this->PosY);
		this->Transform_.Concat();
	}

	arthur[4].cooldown_timer = 20.0f;
}

void King_Arthur::Render(void)
{
	for (char i = 0; i < limit; ++i)
	{
		if (arthur[i].cooldown_timer)
			arthur[i].Render();
	}

	this->GameObject::Render();
}

King_Arthur::~King_Arthur(void)
{
	arthur.clear();
}


namespace
{
	void Move_KA(const float dt, King_Arthur &ka, const Dragon &d)
	{
		UNREFERENCED_PARAMETER(d);
		if (ka.Get_Direction() == RIGHT) // set all attacks to go right
		{
			ka.PosX += ka.GetVelocity().x * dt; // move ka to the right
			(void)ka.Transform_.SetTranslate(ka.PosX, ka.PosY);
		}
		else if (ka.Get_Direction() == LEFT) // set all attacks to go left
		{
			ka.PosX -= ka.GetVelocity().x * dt; // move KA to the left
			(void)ka.Transform_.SetTranslate(ka.PosX, ka.PosY);
		}

		ka.Collision_.Update_Col_Pos(ka.PosX - 30.0f, ka.PosY - 30.0f,  // min point
									 ka.PosX + 30.0f, ka.PosY + 30.0f);	// max point
		
		if (ka.Collision_.Dy_Rect_Rect(d.Collision_, d.GetVelocity(), ka.GetVelocity(), dt))
		{
			ka.Set_Direction(STAY);
		}

		ka.Transform_.Concat();
	}

	void Set_Attack_Dir(King_Arthur & ka)
	{
		
		if (ka.Get_Direction() == RIGHT) // set all attacks to go right
		{
			for (int i = 0; i < limit; ++i)
			{
				arthur[i].SetDir(true);
			}
		}
		else if (ka.Get_Direction() == LEFT) // set all attacks to go left
		{
			for (int i = 0; i < limit; ++i)
			{
				arthur[i].SetDir(false);
			}
		}
	}
}

