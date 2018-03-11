/* Start Header ************************************************************************/
/*!
\file       Mage.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
			Header file for Mage class

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Characters.h"
#include "Dragon.h"
#include "Boss_States.h"


class Mage : public Characters
{
private:
	//------------------------------------------------------------------
	//
	//  BEHAVIOUR FOR MAGE
	//
	//------------------------------------------------------------------
	void Idle(const float dt, const Dragon&);
	void Attack(const float dt, Dragon &);
	void Move(const Dragon &);     // teleport the mage
	void Dead(void);

	bool Line_Of_Sight(const Dragon &);

	Boss_Action_State current_action; // mage behaviour
	bool teleport = true;			  // check if mage teleported 
	Boss_Attack energy_ball;		  // mage's attack
	
public:
	//------------------------------------------------------------------
	//
	//  CONSTRUCTORS FOR MAGE
	//
	//------------------------------------------------------------------
	Mage(const AEVec2&, Sprite*);

	//------------------------------------------------------------------
	//
	//  Functions used to update mage
	//
	//------------------------------------------------------------------
	void Update(Dragon &player, const float dt );
	void Render();

};