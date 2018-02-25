#pragma once
#include "Characters.h"
#include "Dragon.h"
#include "Boss_States.h"


class Mage : public Characters
{
private:
	void Idle(const float dt);
	void Attack(const float dt, const Dragon &);
	void Move(const Dragon &);     // teleport the mage
	void Dead(void);
	bool Line_Of_Sight(const Dragon &);

	Boss_Action_State current_action; // mage behaviour
	bool teleport = true;
	
public:
	//------------------------------------------------------------------
	//
	//  CONSTRUCTORS FOR MAGE
	//
	//------------------------------------------------------------------
	Mage();
	Mage(const float x, const float y);

	//------------------------------------------------------------------
	//
	//  Functions used to update mage
	//
	//------------------------------------------------------------------

	void Update(const float dt, Dragon &player);
	void Update() override {}; // ignored
	void Render();
};