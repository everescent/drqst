#include "Boss_States.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

void Boss_Attack::Update(float dt)  //has to check cooldown_timer
{
	if (cooldown)
	{
		cooldown_timer -= dt;

		if (cooldown_timer <= 0)
		{
			cooldown_timer = 0;
			cooldown = false;
		}
	}


}

void Boss_Attack::Start_Attack(const float x, const float y)
{
	PosX = x;      // start x position of attack
	PosY = y;      // satrt y position of attack
	SetActive(true);         // render attack on screen
	ongoing_attack = true;   // attack is ongoing
}

void Boss_Attack::End_Attack()
{
	//calculate the position and shit for attack
	
	SetActive(false);       // make stab disappaer
	cooldown = true;        // start cooldown
	ResetDist();            // reset distance travled back to 0
	SetCollided(false);     // reset collided flag
	ongoing_attack = false; // attack animation has concluded
}

int Get_Random_Num(int range)
{
	return rand() % (range + 1);
}

void seed_initializer(void) // initialize the seed
{
	srand((unsigned)time(NULL));
}