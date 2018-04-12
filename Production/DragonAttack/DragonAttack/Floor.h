/* Start Header ************************************************************************/
/*!
\file	 Floor.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Floor class body declarations.

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
	Floor class inherits from GameObject class
**************************************************************************************/
class Floor : public GameObject
{
public:
	/**************************************************************************************
	Description:
		Constructor for the Floor class with a sprite pointer and default position.
	p_Sprite:
		The Floor's sprite.
	x:
		The X position.
	y:
		The Y position.
	**************************************************************************************/
	Floor(Sprite* const p_Sprite, float x, float y);
	/**************************************************************************************
	Description:
		Update function for player for the Floor class.
	player:
		The Floor's sprite.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Dragon &player, const float &dt);
	/**************************************************************************************
	Description:
		Update function for AI for the Floor class.
	obj:
		The Floor's sprite.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Characters &obj, const float &dt);
};
