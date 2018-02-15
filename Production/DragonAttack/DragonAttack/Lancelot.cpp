#include "Lancelot.h"
#include "Boss_States.h"
#include <iostream>
#include <vector>

/*
mad enhancement 
move faster
faster attack speed 

phase 2
arondight longer reach?
*/


namespace
{
	std::vector<Boss_Attack> lancelot; // array to store lancelot attack

	Boss_Action_State current_action = ATTACK; // lancelot current action

	const int health = 1500; // health that lancelot start with

	const int phase2_hp = 750; // phase 2 trigger

	const char limit = 3; // num of lancelot attacks

	char behavior_swap = 0; // switch between idling, attacking and moving

	float idle_time = 3.0f; // idling time for lancelot

	void Move_Lancelot(const float dt, Lancelot &ll); // move lancelot towards player

	void Set_Attack_Dir(Lancelot &ll); // set the attack direction of lancelot

}

Lancelot::Lancelot(void)
	: Characters(S_CreateSquare(60.0f, ".//Textures/lancelot.jpg"),
	  health, { 0.0f, 0.0f, 5.0f, 5.0f, Rect }), phase2{ false }, M_E{false}
{
	(void) this->Transform_.SetTranslate(200.0f, Start_Pos_Y); // spawn lancelot at this location
	this->Transform_.Concat();
	this->PosX = 200.0f;      // update lancelot current coordinates
	this->PosY = Start_Pos_Y; // update lancelot current coordinates
	this->SetActive(true);    // spawn lancelot
	this->Set_Direction(LEFT); 
	this->SetVelocity({ 120.0f, 0.0f });    // velocity for lancelot
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	this->Init();
}

void Lancelot::Init()
{
	//initialize lancelot attacks here
	const char* stab = ".//Textures/arondight.png";
	const char* slash = ".//Textures/lancelot_slash.png";

	lancelot.reserve(limit);

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(20.0f, slash), // for stab
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); 

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(20.0f, stab), // for slash
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(0.0f, nullptr),
		Col_Comp()));

	for (char i = 0; i < limit - 1; ++i) // spawning location of slashes
	{
		lancelot[i].PosY = -120.0f;
  (void)lancelot[i].SetVelocity(AEVec2{ 200.0f, 0.0f }); // velocity for slash
		lancelot[i].Transform_.SetScale(3.0f, 2.0f); // determine the size of projectile
		lancelot[i].Transform_.Concat();
	}

	seed_initializer(); // initializes the seed for rng purposes

	ll_attacks[STAB]			= &Lancelot::Stab;              // pointer to slash function
	ll_attacks[SLASH]			= &Lancelot::Slash;			    // pointer to stab function
	ll_attacks[UNIQUE_MECHANIC] = &Lancelot::Mad_Enhancement;	// pointer to enrage function

}

void Lancelot::Idle(const float dt)
{
	static float idle = idle_time; //local variable just for this function

	if (!idle)
	{
		idle = idle_time; // set the idling time
	}

	// change current_action to attack and idle to 0 once idling is over
	idle <= 0 ? current_action = ATTACK, idle = 0 : idle -= dt;

	Set_Attack_Dir(*this); // set the direction of lancelot attacks
}

void Lancelot::Moving(const Dragon &d, const float dt)
{

	static float move_duration = 3.0f;

	if (!move_duration) //set the timer for moving
		move_duration = 3.0f;

	if ((int)(d.PosX - this->PosX) == 0) // character and boss intersect
		this->Set_Direction(STAY);
	else
		//determine if dragon is right or left of king arthur
		(d.PosX - this->PosX) > 0 ? this->Set_Direction(RIGHT) :
		this->Set_Direction(LEFT);

	Move_Lancelot(dt, *this);
	
	//change state to attack once move_duration is 0, reset move_duration
	move_duration <= 0 || this->Get_Direction() == STAY ?
	current_action = ATTACK, move_duration = 0 : move_duration -= dt;

	Set_Attack_Dir(*this);
}

void Lancelot::Update(const Dragon &d, float dt)
{
	if ((int)(d.PosX - this->PosX) == 0) // character and boss intersect
		this->Set_Direction(STAY);
	else
		//determine if dragon is right or left of king arthur
		(d.PosX - this->PosX) > 0 ? this->Set_Direction(RIGHT) :
		this->Set_Direction(LEFT);	
	
	if (this->Get_HP() < phase2_hp)
	{
		Lancelot_Phase2(); // change to phase 2
	}

	if (behavior_swap == 3)
	{
		current_action = MOVING; // switch state to moving
		behavior_swap = 0;       // reset counter
	}

	if (M_E) // off mad enhancement state
	{
		if (lancelot[2].cooldown_timer < 10)
			this->Mad_Enhancement(dt);         
	}

	Set_Attack_Dir(*this); // set direction of attacks

	// switch between boss states
	switch (current_action)
	{
	case IDLE: this->Idle(dt);
		break;

	case MOVING: this->Moving(d, dt);
		break;

	case ATTACK: this->Attack(d, dt);
		break;

	default: break;
	}

	for (char i = 0; i < limit; ++i) // update the skill cooldowns
		lancelot[i].Update(dt);
}

void Lancelot::Attack(const Dragon &d, float dt)
{
	UNREFERENCED_PARAMETER(d);
	UNREFERENCED_PARAMETER(dt);

	Lancelot_Moveset currAttk = STAB;

	if (! lancelot[UNIQUE_MECHANIC].cooldown)
		currAttk = UNIQUE_MECHANIC;
	else if(! lancelot[SLASH].cooldown)
		currAttk =  SLASH;

	(this->*ll_attacks[currAttk])(dt);
}

void Lancelot::Lancelot_Phase2(void)
{
	this->phase2 = true;
	M_E = false;

	const char* tex = ".//Textures/arondight.png";
	lancelot.emplace_back(Boss_Attack(S_CreateSquare(30.0f, tex),
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); // add the 2nd mechanic, still in progress

	ll_attacks[UNIQUE_MECHANIC+1] = &Lancelot::Arondight;
}

void Lancelot::Stab(const float dt)
{// need to account for increase in movement speed
	UNREFERENCED_PARAMETER(dt);
	if (lancelot[0].cooldown)
		return;

	if (!lancelot[0].cooldown_timer)
	{
		lancelot[0].PosX = this->PosX; // start attack from lancelot position
		lancelot[0].SetActive(true);   //render attack on screen
	}

	if (lancelot[0].GetDist() > 100.0f) // range of stab
	{
		lancelot[0].SetActive(false);   // make stab disappaer
		lancelot[0].cooldown = true;    // start cooldown
		lancelot[0].ResetDist();        // reset distance travled back to 0
		lancelot[0].PosX = 0.0f;        // remove flicker
		 
		current_action = IDLE;          // set behavior to idle
		++behavior_swap;                // increment behavior counter

	}

	lancelot[0].Projectile::Pos();
	lancelot[0].Projectile::Update();
	lancelot[0].Render();

	lancelot[0].cooldown_timer = 1.0f;

		
}

void Lancelot::Slash(const float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (lancelot[1].cooldown)
		return;

	if (!lancelot[1].cooldown_timer)
	{
		lancelot[1].PosX = this->PosX; // start attack from lancelot position
		lancelot[1].SetActive(true);   //render attack on screen
	}			 
				 
	if (lancelot[1].GetDist() > 100.0f) // range of slash
	{			 
		lancelot[1].SetActive(false);   // make stab disappaer
		lancelot[1].cooldown = true;    // start cooldown
		lancelot[1].ResetDist();        // reset distance travled back to 0
		lancelot[1].PosX = 0.0f;        // remove flicker

		current_action = IDLE;          // set behavior to idle
		++behavior_swap;                // increment behavior counter

	}

	lancelot[1].Projectile::Pos();
	lancelot[1].Projectile::Update();
	lancelot[1].Render();

	lancelot[1].cooldown_timer = 4.0f;
}

void Lancelot::Mad_Enhancement(const float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (M_E) // mad enhancement is activated
	{
		this->SetVelocity(AEVec2 { 120.0f, 0.0f }); // return to original velocity
		idle_time = 3.0f;							// set the idle time back
		M_E = false;   //turn mad enhancement off
		std::cout << "end\n";
	}
	else
	{
		this->SetVelocity(AEVec2{ 500.0f, 0.0f }); // increase velocity
		lancelot[UNIQUE_MECHANIC].cooldown_timer = 20.0f;        // start cooldown
		lancelot[UNIQUE_MECHANIC].cooldown = true; 
		idle_time = 1.0f;
		M_E = true; 
		std::cout << "start\n";
	}

}

void Lancelot::Arondight(const float dt)
{
	// lancelot pauses for 1 second before unleashing arondight
	static float pause = 1.0f;

	while (pause < 0)
		pause -= dt;


	pause = 2.0f;
}



namespace
{
	void Move_Lancelot(const float dt, Lancelot &l)
	{
		if (l.Get_Direction() == RIGHT) // set all attacks to go right
		{
			l.PosX += l.GetVelocity().x * dt; // move lancelot to the right
			(void)l.Transform_.SetTranslate(l.PosX, l.PosY);
		}
		else if (l.Get_Direction() == LEFT) // set all attacks to go left
		{
			l.PosX -= l.GetVelocity().x * dt; // move lancelot to the left
			(void)l.Transform_.SetTranslate(l.PosX, l.PosY);
		}

		l.Transform_.Concat();
	}
	
	void Set_Attack_Dir(Lancelot &l)
	{
		if (l.Get_Direction() == RIGHT) // set all attacks to go right
		{
			for (int i = 0; i < limit; ++i)
			{
				lancelot[i].SetDir(true);
			}
		}
		else if (l.Get_Direction() == LEFT) // set all attacks to go left
		{
			for (int i = 0; i < limit; ++i)
			{
				lancelot[i].SetDir(false);
			}
		}
	}
}