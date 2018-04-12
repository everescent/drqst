/* Start Header ************************************************************************/
/*!
\file	 Barrier.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Barrier class body declarations.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Dragon.h"
#include "Create_Object.h"

/**************************************************************************************
Description:
	Barrier class inherits from GameObject class
**************************************************************************************/
class Barrier : public GameObject
{
public:
	/**************************************************************************************
	Description:
		Constructor for the Barrier class with a sprite pointer and	default position.
	p_Sprite:
		The Barrier's sprite.
	x:
		The X position.
	y:
		The Y position.
	**************************************************************************************/
	Barrier(Sprite* const p_Sprite, float x, float y);
	/**************************************************************************************
	Description:
		Update function for the Barrier class.
	player:
		This allows access to the player.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Dragon &player, const float &dt);
};