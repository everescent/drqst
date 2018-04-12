/* Start Header ************************************************************************/
/*!
\file	 Tower.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Tower class body declarations.

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
	Tower class inherits from GameObject class
**************************************************************************************/
class Tower : public GameObject
{
public:
	/**************************************************************************************
	Description:
		Constructor for the Tower class with a sprite pointer and default position.
	p_Sprite:
		The Tower's sprite.
	x:
		The X position.
	y:
		The Y position.
	**************************************************************************************/
	Tower(Sprite* const p_Sprite, float x, float y);
	/**************************************************************************************
	Description:
		Update function for the Tower class for player.
	player:
		This allows access to the player.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Dragon &player, const float &dt);
	/**************************************************************************************
	Description:
		Update function for the Tower class for AI.
	obj:
		This allows access to the AI.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Characters &obj, const float &dt);
};
