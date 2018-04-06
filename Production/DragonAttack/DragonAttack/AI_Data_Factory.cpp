/* Start Header ************************************************************************/
/*!
\file       AI_Data_Factory.cpp
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
#include "AI_Data_Factory.h" // header file


namespace
{
	Sprite D_SPRITE;

	Sprite A_SPRITE;
	Sprite G_SPRITE;
	Sprite MA_SPRITE;
	Sprite K_SPRITE;

	Sprite L_SPRITE;
	Sprite ME_SPRITE;
	Sprite KA_SPRITE;
}

/**************************************************************************************
//
// initializes the different sprites we need for the ais
//
**************************************************************************************/
void AI_Sprite_Init(void)
{
	A_SPRITE    = S_CreateSquare(70.0f, ".//Textures/Archer_SpriteSheet.png",     0.2f, 0.33f);
	G_SPRITE    = S_CreateSquare(70.0f, ".//Textures/Grunt_SpriteSheet.png",      0.2f, 0.33f);  
	K_SPRITE    = S_CreateSquare(70.0f, ".//Textures/Knight_SpriteSheet.png",     0.2f, 0.33f);
	MA_SPRITE   = S_CreateSquare(55.0f, ".//Textures/Mage_SpriteSheet.png",       0.2f, 0.5f);
	D_SPRITE    = S_CreateSquare(Dragon_Scale, ".//Textures/Bob_SpriteSheet.png", 0.2f, 0.25f);

	L_SPRITE    = S_CreateSquare(60.0f, ".//Textures/Lancelot_SpriteSheet.png",   0.2f, 0.2f);
    ME_SPRITE   = S_CreateSquare(1.5f, ".//Textures/Merlin_SpriteSheet.png", 0.2f, 0.25f);
	KA_SPRITE   = S_CreateSquare(100.0f, ".//Textures/KA_SpriteSheet.png",        0.2f, 0.33f);


	MA_SPRITE.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	K_SPRITE.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	L_SPRITE.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	KA_SPRITE.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}
/**************************************************************************************
//
// creates a basic ai and return a pointer to it
// initial position is default initialize to spawn outside screen
// if you don't have a fix location at the moment
//
**************************************************************************************/
Characters *Create_Basic_AI(const BASIC_AI mob, const AEVec2& position)
{
	// return the corresponding ai that was given
	switch (mob)
	{
	case GRUNT:  return new Grunt (&G_SPRITE, position.x, position.y);
		break;
	case MAGE:   return new Mage (position, &MA_SPRITE);
		break;
    case ARCHER: return new Archer(&A_SPRITE, Get_Attack_Sprite(ARROW_SPRITE), position.x, position.y);
	    break;
	case KNIGHT: return new Knight(position, &K_SPRITE);
		break;
	case DRAGON: return new Dragon(&D_SPRITE, Get_Attack_Sprite(FIREBALL_SPRITE), position);
		break;
	default: break;
	}
	
	// returns nullptr if no match was found
	return nullptr;
}

/**************************************************************************************
//
// creates a boss ai and return a pointer to it
//
**************************************************************************************/
Characters *Create_Boss_AI(const BOSS_AI boss)
{
	// returns the corresponding boss that was given
	switch (boss)
	{
	case LANCELOT:    return new Lancelot(&L_SPRITE);
		break;
	case MERLIN:      return new Merlin(&ME_SPRITE, Get_Attack_Sprite(EBALL_SPRITE), Get_Attack_Sprite(ARROW_SPRITE));
		break;
	case KING_ARTHUR: return  new King_Arthur(&KA_SPRITE);
		break;

	default: break;
	}
	
	// returns null ptr if no match was found
	return nullptr;
}