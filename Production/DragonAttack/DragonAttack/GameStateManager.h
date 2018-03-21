/* Start Header ************************************************************************/
/*!
\file       GameStateManager.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Manages the game state

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameStateList.h"
#include "Main_Menu.h"
#include "AI_Data_Factory.h"
#include "AI_Attack_Sprites.h"
#include "Test_Stage1.h"
#include "Level_Import.h"
#include "Test_Stage1_2.h"
#include "Test_Stage1_3.h"
//#include "Stage2_1.h"
//#include "Stage2_2.h"
//#include "Stage2_3.h"
#include "Particle_Effects.h"
#include "test_level_import.h"

namespace GSM
{
	// holds the previous, current and next state
	extern GAME_STATE previous, current, next;
	
	/******************************************************************************/
	/*!
	\fn         void GSM_Init

	\brief      Initializes the game state manager

	\param      void

	\retval     void
	No return.
	*/
	/******************************************************************************/
	void GSM_Init(void);

	/******************************************************************************/
	/*!
	\fn         void Init_and_Load

	\brief      Initialize and load the current state

	\param      void

	\retval     void
	No return.
	*/
	/******************************************************************************/
	void Init_and_Load(void);

	/******************************************************************************/
	/*!
	\fn         void Update_and_Draw

	\brief      Update and draw the current state

	\param      dt - delta time to for various formulas

	\retval     void
	No return.
	*/
	/******************************************************************************/
	void Update_and_Draw(float dt);

	/******************************************************************************/
	/*!
	\fn         void Cleanup

	\brief      Free and unload and unload state and move on to the next one

	\param      void

	\retval     void
	No return.
	*/
	/******************************************************************************/
	void Cleanup(void);

	/******************************************************************************/
	/*!
	\fn         void IsGameRunning

	\brief      Checks if the game is still running

	\param      void

	\retval     void
	No return.
	*/
	/******************************************************************************/
	bool IsGameRunning(void);

	// use to call all the functions that need to clear their global variables
	void GSM_Cleanup(void);

}