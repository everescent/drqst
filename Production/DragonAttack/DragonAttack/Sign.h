/* Start Header ************************************************************************/
/*!
\file	 Sign.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Sign class body declarations.

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
	Sign class inherits from GameObject class
**************************************************************************************/
class Sign : public GameObject
{
public:
	/**************************************************************************************
	Description:
		Constructor for the Sign class with a sprite pointer and default position.
	p_Sprite:
		The Sign's sprite.
	x:
		The X position.
	y:
		The Y position.
	**************************************************************************************/
	Sign(Sprite* const p_Sprite, float x, float y);
	/**************************************************************************************
	Description:
		For showing the tutorial message.
	**************************************************************************************/
	bool ShowTutorial;
	/**************************************************************************************
	Description:
		Update function for player for the Sign class.
	player:
		The Sign's sprite.
	dt:
		This is the delta time that is used for calculations.
	**************************************************************************************/
	void Update(Dragon &player, const float &dt);
};
