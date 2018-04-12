/* Start Header ************************************************************************/
/*!
\file       StageManager.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for StageManager

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "GameStateList.h"

namespace SM
{
  /**************************************************************************************
  //
  // Function declarations that will be used in the game
  //
  **************************************************************************************/
  void StageManagerInit(void);

  void          SM_Load                (void);
  void          SM_Init                (void);
  void          SM_Update              (float dt);
  void          SM_Draw                (void);
  void          SM_Unload              (void);
  void          SM_Free                (void);

  void          Set_Next               (STAGE_LIST);
  STAGE_LIST    Get_Next               (void);
  void          Reset                  (STAGE_LIST = STAGE_1_1);
  STAGE_LIST    Get_Curr               (void);
  void	        Set_After_Score        (STAGE_LIST);
  STAGE_LIST    Get_After_Score        (void);
  bool          SubStage_Finished      (void);
}