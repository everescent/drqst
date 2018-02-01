#pragma once
#include <utility>


// States for boss actions
enum Boss_Action_State
{
	IDLE = 0,
	MOVING,
	ATTACK,
	OBSTACLE
};



// A type to store boss attack and the cooldown
struct Boss_Attack : public GameObject
{
	float cooldown_timer;
	bool cooldown;

	Boss_Attack(Sprite&& t_attack, Col_Comp &t_col)
		:cooldown_timer{0}, GameObject(std::move(t_attack), t_col), cooldown{false}
	{
	}

	//void Update() {}; // need to talk to jacob
	//void Update(float dt) {};  has to check cooldown_timer
	void Pos() {};
};