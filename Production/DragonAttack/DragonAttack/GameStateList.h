/* Start Header ************************************************************************/
/*!
\file GameStateList.h
\author William Yoong, william.yoong, 1701674

\par william.yoong\@digipen.edu
\date Jan 24, 2017
\brief
This header file contains the enum for the different states the game haves

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

/* The different game states for the game */
enum GAME_STATE
{
	GS_MAIN = 0,
	GS_LEVEL1_1,
	GS_LEVEL1_2,
	GS_LEVEL1_3,
	GS_LEVEL2_1,
	GS_LEVEL2_2,
	//GS_LEVEL2_3,
	GS_LEVEL_IMPORT,
	AI_TEST_STAGE,  // to be removed before submitting
    
    GS_LEVELS,

	// do not touch this bottom area
	GS_CREDITS,
	GS_QUIT,
	GS_RESTART
};

// the different sub stages for stage manager
enum STAGE_LIST
{
	SUB_STAGE1 = 0,

    STAGE_3_3,
	// don't touch the bottom area, add from top
	SS_SCORE,
	SS_QUIT,
	SS_RESTART
};

// defining a pointer to a void funtion that takes in void
typedef void(*void_fp_void)(void);

// defining a pointer to a void funtion that takes in a float
typedef void(*void_fp_float)(float deltaTime);

// putting all the different functions to one object
struct GameStateFunctions
{
	void_fp_void  Init;
	void_fp_void  Load;
	void_fp_float Update;
	void_fp_void  Draw;
	void_fp_void  Free;
	void_fp_void  Unload;
};