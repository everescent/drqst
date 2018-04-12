/* Start Header ************************************************************************/
/*!
\file	 LevelChangePlatform.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	LevelChangePlatform class body declarations.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Dragon.h"
#include "Create_Object.h"
#include "GameStateManager.h"
#include "StageManager.h"

/**************************************************************************************
Description:
	LevelChangePlatform class inherits from GameObject class
**************************************************************************************/
class LevelChangePlatform : public GameObject
{
public:
	/**************************************************************************************
	Description:
		Constructor for the LevelChangePlatform class with a sprite pointer and 
		default position.
	p_Sprite:
		The LevelChangePlatform's sprite.
	x:
		The X position.
	y:
		The Y position.
	**************************************************************************************/
	LevelChangePlatform(Sprite* const p_Sprite, float x, float y);
	/**************************************************************************************
	Description:
		Update function for the LevelChangePlatform class.
	player:
		This allows access to the player.
	dt:
		This is the delta time that is used for calculations.
	black:
		The black sprite for fading out.
	FadeOut:
		For fading out.
	**************************************************************************************/
	void Update(Dragon &player, const float &dt, Sprite& black, bool& FadeOut);
};
