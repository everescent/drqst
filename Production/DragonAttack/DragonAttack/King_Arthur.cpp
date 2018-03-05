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
#include "AI_Data_Factory.h"
#include "GameStateManager.h"
#include <vector>

namespace {

	std::vector <Boss_Attack> arthur; //an array of boss attacks

	std::vector <Characters*> mobs; //an array to store the mobs to be spawn

	Boss_Action_State current_action = IDLE; // different states of boss arthur

	const int HEALTH    = 300; // initial hp for king arthur
	const int PHASE2_HP = 200; //phase 2 trigger

	const int interval    = 80;  // interval for triple slash
	const int range_limit = 500; // range limit for slash

	const float START_POINT_X = 200.0f; // spawn point of king arthur
	const float START_POINT_Y = -150.0f;// spawn point of king arthur

	const float SLASH_SCALE = 20.0f;

    float left_boundary = -600; // boundaries of charge attack
    float right_boundary = 600; // boundaries of charge attack

	const char limit = 5; // num of king arthur attacks
	const char num_of_mobs = 2; // number of mobs to spawn
	bool spawn_mob = false;

	char behavior_swap = 0; // switch between idling and moving

	bool healing = true; // determine if king arthur is healing

	bool Move_KA(const float dt, King_Arthur &ka, const Dragon &d); // move king arthur towards player
	void Set_Attack_Dir(King_Arthur &ka);							// set the attack directions
	
	enum 
	{
		TOP_LEFT,
		TOP_RIGHT,
		MIDDLE
	};

	struct LOCATION
	{
		AEVec2 min;
		AEVec2 max;
	};

	LOCATION teleport_location[3];
}



King_Arthur::King_Arthur(void)
	: Characters(S_CreateSquare(100.0f, ".//Textures/King_Arthur.png"), HEALTH,
		Col_Comp{ START_POINT_X - 30.0f, START_POINT_Y - 30.0f,
				  START_POINT_X + 30.0f, START_POINT_Y + 30.0f, Rect }),
	phase1{ true }
{
	this->PosX = START_POINT_X;          // change king arthur coordinates to the location set
	this->PosY = START_POINT_Y;          // change king arthur coordinates to the location set
	this->SetActive(true);               // show him on screen
	this->Set_Direction(LEFT);           // set king arthur to face left at the start
	this->SetVelocity({ 120, 0 });       // velocity for king arthur
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	(void)this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.SetScale(-1.0f, 1.0f); // set king arthur to face right at the start
	this->Transform_.Concat();           // spawn king arthur at the location set
	this->Reset_Idle_Time(1.0f);           // duration king arthur will idle
	Init_KA_Attacks();	                 // call initializer for king arthur move set
	Init_MobArray();                     // call initializer for mob array
}
void King_Arthur::Init_KA_Attacks(void)
{
	const char* tex_3s = ".//Textures/holylight.jpg";

	arthur.reserve(limit);

	for(char i = 0; i < limit-1; ++i) // add the single slash and triple slash
		arthur.emplace_back(Boss_Attack(S_CreateSquare(SLASH_SCALE, tex_3s), 
						 Col_Comp(START_POINT_X - SLASH_SCALE, START_POINT_Y - SLASH_SCALE,
								  START_POINT_Y + SLASH_SCALE, START_POINT_Y + SLASH_SCALE, Rect)));


	arthur.emplace_back(Boss_Attack()); // for jump attack since it does not need textures

	for (char i = 0; i < limit - 1; ++i) // initializing other variables in slash
	{
(void)  arthur[i].SetVelocity(AEVec2{ 200.0f, 0.0f }); // velocity for slash
(void)  arthur[i].Transform_.SetScale(1.5f, 1.5f); // determine the size of projectile
(void)	arthur[i].Transform_.SetTranslate( START_POINT_X, START_POINT_Y );
		arthur[i].Transform_.Concat();
	}

	ka_attacks[SINGLE_SLASH]        = &King_Arthur::Single_Slash;
	ka_attacks[TRIPLE_SLASH]        = &King_Arthur::Triple_Slash;
	ka_attacks[UNIQUE_MECHANIC - 2] = &King_Arthur::Dash_Attack;

	seed_initializer();// remove once we finish

}

void King_Arthur::Init_MobArray(void)
{
	for (char i = 0; i < num_of_mobs; ++i)
	{
		// spawn the mobs at the left/right of the screen
		i % 2 ? mobs.push_back(Create_Basic_AI(GRUNT, { -700.0f, START_POINT_Y }))
			  : mobs.push_back(Create_Basic_AI(GRUNT, { 701.0f, START_POINT_Y }));

		// do not render on screen yet
		mobs[i]->SetActive(false);

		// set the blend mode
		mobs[i]->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	}
}

void King_Arthur::Update(Dragon &d, const float dt)
{
	if (this->Get_HP() < PHASE2_HP && phase1) // activate phase 2 once hp drops is 50%
	{
		King_Arthur_Phase2();
	}

	if (behavior_swap == 3)
	{
		current_action = MOVING;  // switch state to moving
		behavior_swap = 0;        // reset the counter
	}

	if (this->Get_HP() <= 0)
	{
		current_action = DEAD; // king arthur has died
	}

	if (spawn_mob) // update the mobs that were spawned
	{
		char i = 0, mobs_dead = 0;
		for (;i < num_of_mobs; ++i)
		{
			if (mobs[i]->IsActive())
			{
				mobs[i]->Update(d, dt);
			}
			else
				++mobs_dead;
		}
		if (mobs_dead == i)
			spawn_mob = false;
	}

	for (char i = 0; i < Bullet_Buffer; ++i) // check for collision with fire balls
		if (d.GetFireball()[i].IsActive())
			if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, this->GetVelocity(), d.GetFireball()[i].GetVelocity(), dt))
			{
				Decrease_HP(d.GetDamage());
				d.GetFireball()[i].SetActive(false);
				healing = false;
			}

	// switch between the boss states
	switch (current_action)
	{
	case IDLE:     this->Idle(d, dt);
		break;

	case MOVING:   this->Moving(d,dt);
		break;

	case OBSTACLE: this->AvoidingObstacle();
		break;

	case ATTACK:   this->Attack(d, dt);
		break;

	case DEAD:    this->Dead();
		break;

	default: break;
	}

	for (char i = 0; i < limit; ++i) // update cooldowns on attacks
			arthur[i].Update(dt);


	
}

void King_Arthur::Idle(const Dragon& d, const float dt)
{
	//changes state to attack once idling is finished, reset idle
	if (Get_Idle_Time() <= 0)
	{
		current_action = ATTACK;
		Reset_Idle_Time(1.0f);

		Set_Forward_Dir(d);
		Set_Attack_Dir(*this);
	}
	else
	{
		Decrease_Idle_Time(dt);
	}
	
}

void King_Arthur::Moving(const Dragon &d, const float dt)
{	
	static float move_duration = 3.0f; // duration KA moves

	Set_Forward_Dir(d);    // set KA to face player
	Move_KA(dt, *this, d); // move king arthur

	//change state to idle if move_duration is 0 or player reached, reset move_duration
	move_duration <= 0 || this->Get_Direction() == STAY ? 
    current_action = IDLE, move_duration = 3.0f : move_duration -= dt;

	Set_Attack_Dir(*this); // set attack directions
}

void King_Arthur::Attack(Dragon &d, const float dt)
{
	static KA_MoveSet currAttk;
		
	//unique mechanic has the highest priority 
	if (! arthur[currAttk].ongoing_attack)
	{
		if (!(arthur[UNIQUE_MECHANIC].cooldown))
			currAttk = (KA_MoveSet) (UNIQUE_MECHANIC - 2);

		//followed by triple slash
		else if (!(arthur[TRIPLE_SLASH].cooldown))
			currAttk = TRIPLE_SLASH;
		else
		{
			currAttk = SINGLE_SLASH; //default attack for king arthur
			arthur[SINGLE_SLASH].Start_Attack(this->PosX, this->PosY);
		}

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

	arthur.pop_back(); // delete jump attack

	const char* tex_ja = ".//Textures/light_texture2320.jpg";
	arthur.emplace_back(Boss_Attack(S_CreateSquare(30.0f, tex_ja),
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); // add the 2nd mechanic, still in progress

	this->phase1 = false; // changed to phase 2

	// change unique mechanism pointer to heal and spawn for phase 2
	ka_attacks[UNIQUE_MECHANIC - 2] = &King_Arthur::Heal_and_Spawn; 

	// platform coordinates to teleport to
	teleport_location[TOP_RIGHT].max.x = 200.0f;
	teleport_location[TOP_RIGHT].max.y = 150.0f;
	
	teleport_location[MIDDLE].max.x    = 00.0f;
	teleport_location[MIDDLE].max.y    = 50.0f;

	teleport_location[TOP_LEFT].max.x  = -200.0f;
	teleport_location[TOP_LEFT].max.y  = 150.0f;
}

void King_Arthur::Dash_Attack(Dragon &d, const float dt)
{	
	if (arthur[UNIQUE_MECHANIC].cooldown) // skill still on cooldown
		return;

	static Direction dash;           // direction KA should dash in

	if (!(arthur[UNIQUE_MECHANIC].ongoing_attack)) // called once to fix dash direction
	{
		dash = this->Get_Direction();
		arthur[UNIQUE_MECHANIC].ongoing_attack = true;
	}

	this->Set_Direction(dash);       // change back the dash direction

	this->SetVelocity({ 800, 0 }); // change king arthur's velocity to dash
	bool hit = Move_KA(dt, *this, d);         // move king arthur towards player

	// need check for collision against dragon
	if(this->Get_Direction() == STAY)
	{
		current_action = IDLE;           // switch current state to idle
		arthur[UNIQUE_MECHANIC].cooldown = true;       // start the cooldown of dash attk
		this->SetVelocity({ 120, 0 });   // reset the velocity of king arthur
		++behavior_swap;    
		arthur[UNIQUE_MECHANIC].ongoing_attack = false;
		arthur[UNIQUE_MECHANIC].cooldown_timer = 10.0f; // cooldown of jump attack

		if(hit)
			d.Decrease_HP();
	}

	this->Transform_.Concat();


}

void King_Arthur::Single_Slash(Dragon &d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);

	arthur[SINGLE_SLASH].Projectile::Update(SLASH_SCALE); // move the slash

	if(! arthur[SINGLE_SLASH].GetCollided()) // check for collision
	{
		if (arthur[SINGLE_SLASH].Collision_.Dy_Rect_Rect(d.Collision_, arthur[SINGLE_SLASH].GetVelocity(), d.GetVelocity(), dt))
		{
			d.Decrease_HP();
			arthur[SINGLE_SLASH].SetCollided(true);
		}
	}

	//disappears once it flys 500 unit away
	if (arthur[SINGLE_SLASH].GetDist() > range_limit) 
	{
		arthur[SINGLE_SLASH].End_Attack();

		current_action = IDLE;                       // set the behaviour to idle
		++behavior_swap;
	}
	

}

void King_Arthur::Triple_Slash(Dragon &d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);

	const char ts_limit = limit - 1; // limit the loop to just the triple slashes
	
	if (!(arthur[TRIPLE_SLASH].ongoing_attack)) // sets the attack to start from KA
	{
		for (char i = 1; i < ts_limit; ++i)
		{
			arthur[i].SetPos(this->PosX, this->PosY);
		}
		arthur[TRIPLE_SLASH].SetActive(true);         // set the first attack to show
		arthur[TRIPLE_SLASH].ongoing_attack = true;   // attack is currently ongoing
	}


	//slashes 3 times at the player with an interval inbetween
	for (char i = 1; i < ts_limit; ++i)
	{		
		// release the 2nd and third projectile after the one in front travels a certain distance
		if (i > 1 && arthur[i-1].GetDist() > interval)
		{
			arthur[i].SetActive(true);
		}

		arthur[i].Projectile::Update(SLASH_SCALE); // update the pos of the slash
		
		// checks if it has collided with the player
		if (!arthur[i].GetCollided())
		{
			if (arthur[i].Collision_.Dy_Rect_Rect(d.Collision_, arthur[i].GetVelocity(), d.GetVelocity(), dt))
			{
				d.Decrease_HP();
				for (char j = 1; j < ts_limit; ++j)
					arthur[j].SetCollided(true); // set collided flag to true
			}
		}

		if (arthur[i].GetDist() > range_limit) // makes projectiles disappear
		{
			arthur[i].SetActive(false); // disappears from screen
			arthur[i].ResetDist();      // reset the projectile distance traveled
			arthur[i].PosX = 0.0f;      // remove flicker

			if (i == 3) // last slash has completed
			{
				current_action                      = IDLE;     // change state to idle
				arthur[TRIPLE_SLASH].cooldown       = true;     // start cooldown		
				arthur[TRIPLE_SLASH].ongoing_attack = false;    // attack has complete
				arthur[TRIPLE_SLASH].cooldown_timer = 5.0f;     // cooldown duration
				++behavior_swap;								// behavior counter

				for (char j = 1; j < ts_limit; ++j)
					arthur[j].SetCollided(false);
			}
		}
	}

}


void King_Arthur::Heal_and_Spawn(Dragon &d, const float dt)
{
	UNREFERENCED_PARAMETER(d);
	//teleport to a platform, heal and spawn mobs

	if (!arthur[UNIQUE_MECHANIC].ongoing_attack) // teleport once
	{
		//get a random number and teleport to the coresponding platform
		switch (Get_Random_Num(2))
		{
		case TOP_RIGHT: this->PosX = teleport_location[TOP_RIGHT].max.x;
						this->PosY = teleport_location[TOP_RIGHT].max.y;
						
						left_boundary  = teleport_location[TOP_RIGHT].max.x - 100.0f;
						right_boundary = teleport_location[TOP_RIGHT].max.x + 100.0f;
			break;

		case MIDDLE: this->PosX = teleport_location[MIDDLE].max.x;
					 this->PosY = teleport_location[MIDDLE].max.y;

					 left_boundary  = teleport_location[MIDDLE].max.x - 100.0f;
					 right_boundary = teleport_location[MIDDLE].max.x + 100.0f;
			break;

		case TOP_LEFT: this->PosX = teleport_location[TOP_LEFT].max.x;
					   this->PosY = teleport_location[TOP_LEFT].max.y;

					   left_boundary  = teleport_location[TOP_LEFT].max.x - 100.0f;
					   right_boundary = teleport_location[TOP_LEFT].max.x + 100.0f;
			break;

		default: break;
		}
		this->Transform_.SetTranslate(this->PosX, this->PosY);
		this->Transform_.Concat();
		healing = true;
		arthur[UNIQUE_MECHANIC].ongoing_attack = true;
		spawn_mob = true;

		for (char i = 0; i < num_of_mobs; ++i)
			mobs[i]->SetActive(true);
	}

	if (healing && Get_HP() != HEALTH)
	{
		static float timer = 0.5f;

		timer <= 0 ? Set_HP(Get_HP() + 10), timer = 0.5f : timer -= dt;

		std::cout << Get_HP() << std::endl;
	}
	else
	{
		arthur[UNIQUE_MECHANIC].cooldown       = true;
		arthur[UNIQUE_MECHANIC].ongoing_attack = false;		
		arthur[UNIQUE_MECHANIC].cooldown_timer = 20.0f;
	}

}

void King_Arthur::Render(void)
{
	for (char i = 0; i < limit; ++i)
	{
		if (arthur[i].IsActive())
			arthur[i].Render();
	}

	for (char i = 0; i < num_of_mobs; ++i)
	{
		if (mobs[i]->IsActive())
			mobs[i]->Render();
	}

	this->GameObject::Render(); // render king arthur on screen
}

void King_Arthur::Dead(void)
{
	SetActive(false);
	// play some animation. Camera shake?

	for (char i = 0; i < num_of_mobs; ++i)
		mobs[i]->SetActive(false);

	//GSM::next = GS_CREDITS;
}

void King_Arthur::Set_Forward_Dir(const Dragon& d)
{
	if ((d.PosX - this->PosX) > 0)
	{
		this->Set_Direction(RIGHT);
		this->Transform_.SetScale(1.0f, 1.0f); // reflect the texture to face player
	}
	else
	{
		this->Set_Direction(LEFT);
		this->Transform_.SetScale(-1.0f, 1.0f); // reflect the texture to face player
	}
	this->Transform_.Concat();
}

King_Arthur::~King_Arthur(void)
{
	arthur.clear();
	mobs.clear();
}


namespace
{
	bool Move_KA(const float dt, King_Arthur &ka, const Dragon &d)
	{
		if (ka.PosX < right_boundary && ka.PosX > left_boundary)
		{
			if (ka.Get_Direction() == RIGHT) 
			{
				ka.PosX += ka.GetVelocity().x * dt; // move ka to the right
			}
			else if (ka.Get_Direction() == LEFT) 
			{
				ka.PosX -= ka.GetVelocity().x * dt; // move KA to the left			
			}

			(void)ka.Transform_.SetTranslate(ka.PosX, ka.PosY);
			ka.Collision_.Update_Col_Pos(ka.PosX - 30.0f, ka.PosY - 30.0f,  // min point
										 ka.PosX + 30.0f, ka.PosY + 30.0f);	// max point
		}

		else
		{
			ka.Set_Direction(STAY);

			// snap king arthur
			ka.PosX > right_boundary ? ka.PosX -= 5.0f : ka.PosX += 5.0f;
		}

		ka.Transform_.Concat();

		if (ka.Collision_.Dy_Rect_Rect(d.Collision_, d.GetVelocity(), ka.GetVelocity(), dt))
		{
			ka.Set_Direction(STAY);
			return true;
		}

		return false;
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

