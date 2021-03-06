/* Start Header ************************************************************************/
/*!
\file       GameStateList.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for GameStateList

All content � 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once

/**************************************************************************************
//
// enumeration for the various game states
//
**************************************************************************************/
enum GAME_STATE
{
  GS_MAIN = 0,
  GS_LEVEL1_1,
  GS_LEVEL1_2,
  GS_LEVEL1_3,
  GS_LEVEL2_1,
  GS_LEVEL2_2,
  GS_LEVEL2_3,
  GS_LEVEL3_1,
  GS_LEVEL3_2,
  GS_LEVEL3_3,
  GS_LEVEL_IMPORT,
  AI_TEST_STAGE,  // to be removed before submitting
    
  GS_LEVELS,
  GS_LEVEL_SELECTOR,
  // do not touch this bottom area
  GS_CREDITS,
  GS_QUIT,
  GS_RESTART
};

/**************************************************************************************
//
// enumeration for the various stage states
//
**************************************************************************************/
enum STAGE_LIST
{
  SUB_STAGE1 = 0,

  STAGE_1_1,
  STAGE_1_2,
  STAGE_1_3,
  STAGE_2_1,
  STAGE_2_2,
  STAGE_2_3,
  STAGE_3_1,
  STAGE_3_2,
  STAGE_3_3,

  // don't touch the bottom area, add from top
  SS_SCORE,
  SS_LOAD,
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