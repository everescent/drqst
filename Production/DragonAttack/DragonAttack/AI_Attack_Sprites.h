/* Start Header ************************************************************************/
/*!
\file       AI_Attack_Sprites.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates ais base on the enum that was passed in

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Sprite.h"
#include "Create_Object.h"

/**************************************************************************************
//
// enumeration for the  attack sprites belonging to basic ais
//
**************************************************************************************/
enum ATTACK_SPRITE
{
	EBALL_SPRITE,
	STAB_SPRITE,
	ARROW_SPRITE,

	FIREBALL_SPRITE
};

/**************************************************************************************
//
// initializes the different sprites we need for the attacks
//
**************************************************************************************/
void Attack_Sprite_Init(void);

/**************************************************************************************
//
// returns the corresponding attack sprite that was given in the parameter
//
**************************************************************************************/
Sprite* Get_Attack_Sprite(const ATTACK_SPRITE);