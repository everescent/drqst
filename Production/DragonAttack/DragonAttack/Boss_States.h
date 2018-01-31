#pragma once


// States for boss actions
enum BossActionState
{
	Idle = 0,
	Moving,
	AvoidingObstacle,
	Attacking
};



// A type to store boss attack and the cooldown
struct Boss_Attack : public GameObject
{
	float cooldown_timer;


	Boss_Attack()
		:cooldown_timer{0}
	{
	}

	void Update() {};
	void Pos() {};
};