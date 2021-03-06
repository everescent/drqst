/* Start Header ************************************************************************/
/*!
\file       GameStateManager.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for GameStateManager

All content � 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameStateList.h"
#include "Main_Menu.h"
#include "Level_Selector.h"
#include "AI_Data_Factory.h"
#include "AI_Attack_Sprites.h"
#include "Level_Import.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"
#include "Stage2_1.h"
#include "Stage2_2.h"
#include "Stage2_3.h"
#include "Stage3_1.h"
#include "Stage3_2.h"
#include "Stage3_3.h"
#include "Particle_Effects.h"
#include "Credits.h"
#include "test_level_import.h"

namespace GSM
{
	/**************************************************************************************
	//
	// holds the previous, current and next game state. controls the game flow
	//
	**************************************************************************************/
	extern GAME_STATE previous, current, next;
	
	/**************************************************************************************
	//
	// Function declarations
	//
	**************************************************************************************/
	void GSM_Init(void);
	void Init_and_Load(void);
	void Update_and_Draw(float dt);
	void Cleanup(void);
	bool IsGameRunning(void);
	void GSM_Cleanup(void);

}