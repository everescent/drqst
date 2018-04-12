/* Start Header ************************************************************************/
/*!
\file    Grunt.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
Archer class functions declarations here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "Characters.h"
#include "Dragon.h"
#include "Boss_States.h"

// Grunt class inherits from Characters class
class Grunt : public  Characters
{
private:
	/* Behaviours */
	void MoveTowardPlayer(const Dragon &d, const float dt);
	void Idle(const Dragon &d, const float dt);

	/* Helper Function */
	void Set_Facing_Dir(const Dragon &d);
	bool LineOfSight(const Dragon &d);

	/* Variables */
	Animation anim;
	Boss_Action_State current_action;
	static Audio_Engine SFX;
	bool  Knockback = false;
	float posit_tmp = 0.0f;
public:
	// Grunt constructor
	Grunt(Sprite *p_Sprite, const float posX = 0.0f, const float posY = 0.0f);
	// Grunt destructor
    ~Grunt();
	// Grunt update function
	void Update(Dragon &d, const float dt);
	// Grunt Mute function
    void Mute();
	// Grunt Unmute function
    void Unmute();
};
