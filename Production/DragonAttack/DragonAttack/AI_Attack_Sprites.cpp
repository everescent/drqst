/* Start Header ************************************************************************/
/*!
\file       AI_Attack_Sprites.cpp
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
#include "AI_Attack_Sprites.h" // header file

namespace // for global variables
{
	Sprite EBALL_ATTACK;
	Sprite STAB_ATTACK;
	Sprite ARROW_ATTACK;
	Sprite FIREBALL_ATTACK;
}

/**************************************************************************************
//
// initializes the different sprites we need for the attacks
//
**************************************************************************************/
void Attack_Sprite_Init(void)
{
	EBALL_ATTACK    = S_CreateSquare(20.0f, ".//Textures/Energy_Ball_Final.png");
	STAB_ATTACK     = S_CreateSquare(40.0f, ".//Textures/Knight_Sword.png");
	ARROW_ATTACK    = S_CreateSquare(20.0f, ".//Textures/Arrow.png");
	FIREBALL_ATTACK = S_CreateSquare(50.0f, ".//Textures/Fireball.png");

	EBALL_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	STAB_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	ARROW_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	FIREBALL_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

/**************************************************************************************
//
// returns the corresponding attack sprite that was given in the parameter
//
**************************************************************************************/
Sprite* Get_Attack_Sprite(const ATTACK_SPRITE ATTACK)
{
	switch (ATTACK)
	{
	case EBALL_SPRITE:return &EBALL_ATTACK;
		break;
	case STAB_SPRITE: return &STAB_ATTACK;
		break;
	case ARROW_SPRITE: return &ARROW_ATTACK;
		break;
	case FIREBALL_SPRITE: return &FIREBALL_ATTACK;
		break;
	default: return nullptr;
	}
}

