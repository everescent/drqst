#include "Lancelot.h"
#include "Boss_States.h"
#include <iostream>
#include <vector>

/*
@TODO:
slash coordinates. Swing top down, down up. Has an interval of 1 second i guess
Throw Weapon : but unavailable during mad enhancement
ARONDIGHT : phase 2 mechanic

collision with fireball and lancelot
*/


namespace
{
	enum Lancelot_Moveset
	{
		STAB,
		SLASH,
		MAD_ENHANCEMENT,
		ARONDIGHT
	};
	
	std::vector<Boss_Attack> lancelot; // array to store lancelot attack

	Boss_Action_State current_action = ATTACK; // lancelot current action

	const int HEALTH = 1500; // health that lancelot start with
	const int PHASE2_HP = 750; // phase 2 trigger

	const char limit = 4; // num of lancelot attacks

	float idle_time          = 3.0f; // idling time for lancelot
	const float ATTACK_RANGE = 50.0f;
	const float ATTACK_SCALE = 20.0f;
	static Lancelot_Moveset currAttk = STAB;
}

Lancelot::Lancelot(void)
	: Characters(S_CreateSquare(60.0f, ".//Textures/lancelot.jpg"),
	  HEALTH, { 0.0f, 0.0f, 5.0f, 5.0f, Rect }),  M_E{false}
{
	(void) this->Transform_.SetTranslate(200.0f, Start_Pos_Y); // spawn lancelot at this location
	this->Transform_.Concat();
	this->SetPos( 200.0f , Start_Pos_Y);    // update lancelot current coordinates
	this->SetActive(true);					// spawn lancelot
	this->Set_Direction(LEFT);				// face left
	this->SetVelocity({ 120.0f, 0.0f });    // velocity for lancelot
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	this->Init();							// initialize the attacks lancelot have
}

void Lancelot::Init()
{
	//initialize lancelot attacks here
	const char* stab = ".//Textures/arondight.png";
	const char* slash = ".//Textures/lancelot_slash.png";
	const char* tex = ".//Textures/arondight.png";

	lancelot.reserve(limit);

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(ATTACK_SCALE, slash), // for stab
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); 

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(ATTACK_SCALE, stab), // for slash
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	lancelot.emplace_back(Boss_Attack());                                // mad enhancement

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(ATTACK_SCALE, tex), // arondight
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); 

	for (char i = 0; i < limit - 1; ++i) // spawning location of attacks
	{
		lancelot[i].PosY = -120.0f;                      // y coordinates of attacks
  (void)lancelot[i].SetVelocity(AEVec2{ 200.0f, 0.0f }); // velocity for slash
		lancelot[i].Transform_.SetScale(3.0f, 2.0f);     // determine the size of projectile
		lancelot[i].Transform_.Concat();
		lancelot[i].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	}

	lancelot[MAD_ENHANCEMENT].cooldown_timer = 10.0f;  // prevent unique mechanic from activating at the start of fight
	
	seed_initializer(); // initializes the seed for rng purposes

}

void Lancelot::Idle(const Dragon& d, const float dt)
{
	static float idle = idle_time; //local variable just for this function

	if (!idle)
	{
		idle = idle_time; // set the idling time
	}

	// change current_action to attack and idle to 0 once idling is over
	if (idle <= 0)
	{
		current_action = d.PosX - this->PosX <= ATTACK_RANGE ? ATTACK : MOVING;
		idle = 0;
		Set_Attk_Dir();
	}
	else
		idle -= dt;
}

void Lancelot::Moving(const Dragon &d, const float dt)
{
	// change the direction lancelot is facing

	if ((d.PosX - this->PosX) == ATTACK_RANGE) // character and boss intersect
		this->Set_Direction(STAY);
	else
		//determine if dragon is right or left of king arthur
		(d.PosX - this->PosX) > 0 ? this->Set_Direction(RIGHT) 
								  : this->Set_Direction(LEFT);

	if (Get_Direction() == RIGHT)     
	{
		PosX += GetVelocity().x * dt; // move lancelot to the right
	}
	else if (Get_Direction() == LEFT) 
	{
		PosX -= GetVelocity().x * dt; // move lancelot to the left
	}

	(void)Transform_.SetTranslate(PosX, PosY);
	Transform_.Concat();
	
	//change state to attack once move_duration is 0, reset move_duration
	if (this->Get_Direction() == STAY)
	{
		current_action = ATTACK;
		Set_Attk_Dir();
	}
}

void Lancelot::Update(Dragon &d, float dt)
{
	
	if (this->Get_HP() < PHASE2_HP && phase == PHASE_1)
	{
		Lancelot_Phase2(); // change to phase 2
	}


	if (M_E) // off mad enhancement state
	{
		if (lancelot[MAD_ENHANCEMENT].cooldown_timer < 10)
			this->Mad_Enhancement();         
	}

	// switch between boss states
	switch (current_action)
	{
	case IDLE: this->Idle(d, dt);
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

void Lancelot::Render(void)
{
	GameObject::Render();
	lancelot[currAttk].Render();
}

void Lancelot::Attack(Dragon &d, const float dt)
{
	//static const unsigned MAD_ENHANCEMENT_NOT_CD = 1;
	//static const unsigned ARONDIGHT_NOT_CD = 2;
	//static const unsigned SLASH_NOT_CD = 1 << 3;
	//static const unsigned ALL_ON_CD = 1 << 4;

	if (!lancelot[currAttk].ongoing_attack)
	{
		if(phase & PHASE_1 && ! lancelot[MAD_ENHANCEMENT].cooldown)
			currAttk = MAD_ENHANCEMENT;

		else if (phase & PHASE_2 && ! lancelot[ARONDIGHT].cooldown)
			currAttk = ARONDIGHT;

		else if (! lancelot[SLASH].cooldown)
			currAttk = SLASH;

		else
			currAttk = STAB;
	}

	switch (currAttk)
	{
	case STAB: Stab(d, dt);
		break;
	case SLASH: Slash(d, dt);
		break;
	case MAD_ENHANCEMENT: Mad_Enhancement();
		break;
	case ARONDIGHT:
		break;
	}

}

void Lancelot::Lancelot_Phase2(void)
{
	phase = PHASE_2;
	M_E = false;
}

void Lancelot::Stab(Dragon& d, const float dt)
{// need to account for increase in movement speed
	UNREFERENCED_PARAMETER(dt);

	if (! lancelot[STAB].ongoing_attack)
	{
		lancelot[STAB].PosX = this->PosX;      // start attack from lancelot position
		lancelot[STAB].SetActive(true);        // render attack on screen
		lancelot[STAB].ongoing_attack = true;  // attack is ongoing
	}

	lancelot[STAB].Projectile::Update(ATTACK_SCALE);

	if (!lancelot[STAB].GetCollided())
	{
		if (lancelot[STAB].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[STAB].GetVelocity(), d.GetVelocity(), dt))
		{
			lancelot[STAB].SetCollided(true);
			d.Decrease_HP();
		}
	}

	if (lancelot[STAB].GetDist() > 100.0f) // range of stab
	{
		lancelot[STAB].SetActive(false);   // make stab disappaer
		lancelot[STAB].cooldown = true;    // start cooldown
		lancelot[STAB].ResetDist();        // reset distance travled back to 0
		lancelot[STAB].SetCollided(false); // reset collided flag

		current_action = IDLE;             // set behavior to idle
	}

		
}

void Lancelot::Slash(Dragon& d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);

	static float slash_interval;

	if (!lancelot[SLASH].ongoing_attack)
	{
		lancelot[SLASH].PosX = this->PosX;       // start attack from lancelot position
		lancelot[SLASH].SetActive(true);         // render attack on screen
		lancelot[SLASH].ongoing_attack = true;   // attack currently ongoing
		slash_interval = 1.0f;
	}			 
		
	lancelot[SLASH].Projectile::Update(ATTACK_SCALE);

	if (!lancelot[SLASH].GetCollided())
	{
		if (lancelot[SLASH].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[SLASH].GetVelocity(), d.GetVelocity(), dt))
		{
			lancelot[SLASH].SetCollided(true);
			d.Decrease_HP();
		}
	}

	if (lancelot[SLASH].GetDist() > 100.0f) // range of slash
	{			 
		lancelot[SLASH].SetActive(false);   // make stab disappaer
		lancelot[SLASH].cooldown = true;    // start cooldown
		lancelot[SLASH].ResetDist();        // reset distance travled back to 0
		lancelot[SLASH].cooldown_timer = M_E ? 2.0f : 4.0f; // shorter cooldown when berserked
		lancelot[SLASH].SetCollided(false);					// reset collision flag

		current_action = IDLE;          // set behavior to idle
		slash_interval = 1.0f;
	}




}

void Lancelot::Mad_Enhancement()
{
	if (M_E) // mad enhancement is activated
	{
		this->SetVelocity(AEVec2 { 120.0f, 0.0f }); // return to original velocity
		idle_time = 1.0f;							// set the idle time back
		M_E = false;								// turn mad enhancement off
		std::cout << "end\n";
	}
	else
	{
		this->SetVelocity(AEVec2{ 500.0f, 0.0f });               // increase velocity
		lancelot[MAD_ENHANCEMENT].cooldown_timer = 20.0f;        // start cooldown
		lancelot[MAD_ENHANCEMENT].cooldown = true;               // cooldown duration
		idle_time = 0.0f;                                        // no idle interval
		M_E = true;												 // mad enhancement flag
		std::cout << "start\n";
	}

}

void Lancelot::Arondight(Dragon& d, const float dt)
{
	UNREFERENCED_PARAMETER(d);
	UNREFERENCED_PARAMETER(dt);
	
	// lancelot pauses for 1 second before unleashing arondight
	static float pause = 1.0f;

	while (pause < 0)
		pause -= dt;


	pause = 2.0f;
}

void Lancelot::Set_Attk_Dir()
{
	if (Get_Direction() == RIGHT)            // set all attacks to go right
	{
		for (int i = 0; i < limit; ++i)
		{
			lancelot[i].SetDir(true);
		}
	}
	else                                     // set all attacks to go left
	{
		for (int i = 0; i < limit; ++i)
		{
			lancelot[i].SetDir(false);
		}
	}
}


Lancelot::~Lancelot()
{
	lancelot.clear();
}

