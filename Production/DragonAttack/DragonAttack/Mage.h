#pragma once
#include "Characters.h"
#include "Dragon.h"
#include "Boss_States.h"


class Mage : public Characters
{
private:
	void Idle(const float dt, const Dragon&);
	void Attack(const float dt, Dragon &);
	void Move(const Dragon &);     // teleport the mage
	void Dead(void);

	bool Line_Of_Sight(const Dragon &);

	Boss_Action_State current_action; // mage behaviour
	bool teleport = true;
	Boss_Attack energy_ball;
	
public:
	//------------------------------------------------------------------
	//
	//  CONSTRUCTORS FOR MAGE
	//
	//------------------------------------------------------------------
	Mage();
	Mage(const AEVec2&);

	//------------------------------------------------------------------
	//
	//  Functions used to update mage
	//
	//------------------------------------------------------------------

	void Update(Dragon &player, const float dt );
	void Render();

};