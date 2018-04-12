/* Start Header ************************************************************************/
/*!
\file    Grunt.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Grunt class body declarations.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "Characters.h"
#include "Dragon.h"
#include "Boss_States.h"

/**************************************************************************************
Description:
	Grunt class inherits from Characters class
**************************************************************************************/
class Grunt : public  Characters
{
private:
	/**************************************************************************************
	Description:
		This moves the Grunt towards to player.
	d:
		This allows access to the player's position.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void MoveTowardPlayer(const Dragon &d, const float dt);
	/**************************************************************************************
	Description:
		This puts the Grunt into an idle state that constantly checks if 
		the player is back within range.
	d:
		This allows access to the player's position.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Idle(const Dragon &d, const float dt);
	/**************************************************************************************
	Description:
		This sets the current direction that the Grunt faces.
	d:
		This allows access to the player's position.
	**************************************************************************************/
	void Set_Facing_Dir(const Dragon &d);
	/**************************************************************************************
	Description:
		This returns true of false based on whether the distance between the 
		Grunt and player is within a certain range.
	d:
		This allows access to the player's position.
	**************************************************************************************/
	bool LineOfSight(const Dragon &d);

	/* Variables */
	Animation anim;                   // Current animation state of the Grunt.
	Boss_Action_State current_action; // Current state of the Grunt.
	static Audio_Engine SFX;          // Soldier sound effects.

public:
	/**************************************************************************************
	Description:
		Constructor for the Grunt class with a sprite pointer and default position.
	p_Sprite:
		The Grunt's sprite.
	posX:
		The default X position.
	posY:
		The default Y position.
	**************************************************************************************/
	Grunt(Sprite *p_Sprite, const float posX = 0.0f, const float posY = 0.0f);
	/**************************************************************************************
	Description:
		Deletes the Grunt sprite.
	**************************************************************************************/
    ~Grunt();
	/**************************************************************************************
	Description:
		Update function for the Grunt class.
	d:
		Player reference for checking position.
	dt:
		Delta time used for calculations.
	**************************************************************************************/
	void Update(Dragon &d, const float dt);
	/**************************************************************************************
	Description:
		Mutes the sound effects for the Grunt.
	**************************************************************************************/
    void Mute();
	/**************************************************************************************
	Description:
		Unmutes the sound effects for the Grunt.
	**************************************************************************************/
    void Unmute();
};
