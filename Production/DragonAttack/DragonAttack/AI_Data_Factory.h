/* Start Header ************************************************************************/
/*!
\file       AI_Data_Factory.h
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
#include "Archer.h"
#include "Grunt.h"
#include "Mage.h"
#include "Merlin.h"
#include "King_Arthur.h"
#include "Knight.h"
#include "Lancelot.h"
#include "AI_Attack_Sprites.h"

/**************************************************************************************
//
// enumurator for the different ais we have
//
**************************************************************************************/
enum BASIC_AI
{
	GRUNT,
	MAGE,
    ARCHER,
	KNIGHT,
	// add any mobs that you have created here

	DRAGON
};

/**************************************************************************************
//
// enumurator for the different bosses we have
//
**************************************************************************************/
enum BOSS_AI
{
	LANCELOT,
	MERLIN,
	KING_ARTHUR

	// add any boss you create here
};

/**************************************************************************************
//
// initializes the different sprites we need for the ais
// CALLED ONCE AND ONLY ONCE AT THE START OF THE GAME
//
**************************************************************************************/
void AI_Sprite_Init(void);

/**************************************************************************************
//
// creates a basic ai and return a pointer to it
// initial position is default initialize to spawn outside screen
// if you don't have a fix location at the moment
//
**************************************************************************************/
Characters *Create_Basic_AI(const BASIC_AI mob_name, const AEVec2 &initial_position = { -700.0f, -400.0f });


/**************************************************************************************
//
// creates a boss ai and return a pointer to it
//
**************************************************************************************/
Characters *Create_Boss_AI(const BOSS_AI boss_name);