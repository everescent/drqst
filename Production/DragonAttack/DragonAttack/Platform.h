/* Start Header ************************************************************************/
/*!
\file	 Platform.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Platform class body declarations.

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
	Platform class inherits from GameObject class
**************************************************************************************/
class Platform : public GameObject
{
public:
	/**************************************************************************************
	Description:
		Constructor for the Platform class with a sprite pointer and default position.
	p_Sprite:
		The Platform's sprite.
	x:
		The X position.
	y:
		The Y position.
	**************************************************************************************/
	Platform(Sprite* const p_Sprite, float x, float y);
	/**************************************************************************************
	Description:
		Update function for player for the Platform class.
	player:
		The Platform's sprite.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Dragon &player, const float &dt);
	/**************************************************************************************
	Description:
		Update function for AI for the Platform class.
	obj:
		The Platform's sprite.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Characters &player, const float &dt);
};
