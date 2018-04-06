/* Start Header ************************************************************************/
/*!
\file       Grunt.h
\author     Andrew Chong Jiahao
\par email: c.jiahaoandrew\@digipen.edu
\brief
Grunts to be created for the game.

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

class Grunt : public  Characters
{
private:
	/* Behaviours */
	void MoveTowardPlayer(const Dragon &d, const float dt);
	//void AttackPlayer(const Dragon &d);
	void Idle(const Dragon &d, const float dt);

	/* Helper Function */
	void Set_Facing_Dir(const Dragon &d);
	bool LineOfSight(const Dragon &d);

	/* Variables */
	Animation anim;
	Boss_Action_State current_action;
	static Audio_Engine SFX;
	bool  PlayerSeen;
	bool  PlayerInRange;
	float MovementX;
	float MovementY;
	float moveSpd;
	int   EstIdleX;
	bool  Knockback = false;
	float posit_tmp = 0.0f;
	float distance = 0.0f;
public:
	/******************************************************************************/
	/*!
	\fn         Grunt

	\brief      Constructor for a Grunt Object

	\param      float x, float y
	- x and y coordinates of the object

	\retval     void
	No return.
	*/
	/******************************************************************************/
	Grunt(Sprite *p_Sprite, const float posX = 0.0f, const float posY = 0.0f);
    ~Grunt() = default;
	/******************************************************************************/
	/*!
	\fn         Update

	\brief      Update function for a Grunt Object

	\param      Dragon &d, const float dt
	d
	- reference to player
	dt
	- delta time

	\retval     void
	No return.
	*/
	/******************************************************************************/
	void Update(Dragon &d, const float dt);
};
