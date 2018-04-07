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
#include "Characters.h"   // parent class
#include "Dragon.h"       // player class
#include "Boss_States.h"  // ai states

/**************************************************************************************
//
// A class that creates a mage object
//
**************************************************************************************/

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
	void Move(const Dragon &);                
	void Dead(void);

	//------------------------------------------------------------------
	//
	//  HELPER FUNCTIONS
	//
	//------------------------------------------------------------------
	bool Line_Of_Sight(const Dragon &);

	//------------------------------------------------------------------
	//
	//  MAGE VARIABLES
	//
	//------------------------------------------------------------------
	Boss_Action_State current_action;     // mage behaviour
	bool              teleport;	          // check if mage teleported 
	Boss_Attack       energy_ball;		  // mage's attack
	Animation         anim;               // animations for the mage
	
public:
	//------------------------------------------------------------------
	//
	//  CONSTRUCTORS FOR MAGE
	//
	//------------------------------------------------------------------
	Mage(const AEVec2&, Sprite*);
    ~Mage();
	//------------------------------------------------------------------
	//
	//  Functions used to update mage
	//
	//------------------------------------------------------------------
	void Update(Dragon &player, const float dt );
	void Render();
    void Renew_Mage(const AEVec2 & newPos);

};