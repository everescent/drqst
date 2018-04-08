/* Start Header ************************************************************************/
/*!
\file       StageManager.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief


Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"
#include "Stage2_1.h"
#include "Stage2_2.h"
#include "Stage2_3.h"
#include "Stage3_1.h"
#include "Stage3_2.h"
#include "Stage3_3.h"
#include "Score_Page.h"

// global variables that will be used in this stage manager
static STAGE_LIST s_previous, s_current, s_next, s_after_score;

namespace SM
{
  // create an array of function pointers for the stage manager
  GameStateFunctions StageManager[SS_QUIT] = { nullptr };

  /**************************************************************************************
  //
  // Initialize the stage manager variables
  //
  **************************************************************************************/
  void StageManagerInit()
  {
    StageManager[SUB_STAGE1].Init     = nullptr;
    StageManager[SUB_STAGE1].Load     = nullptr;
    StageManager[SUB_STAGE1].Update   = nullptr;
    StageManager[SUB_STAGE1].Draw     = nullptr;
    StageManager[SUB_STAGE1].Free     = nullptr;
    StageManager[SUB_STAGE1].Unload   = nullptr;

    StageManager[STAGE_1_1].Init      = Stage1_1::Init;
    StageManager[STAGE_1_1].Load      = Stage1_1::Load;
    StageManager[STAGE_1_1].Update    = Stage1_1::Update;
    StageManager[STAGE_1_1].Draw      = Stage1_1::Draw;
    StageManager[STAGE_1_1].Free      = Stage1_1::Free;
    StageManager[STAGE_1_1].Unload    = Stage1_1::Unload;

    StageManager[STAGE_1_2].Init      = Stage1_2::Init;
    StageManager[STAGE_1_2].Load      = Stage1_2::Load;
    StageManager[STAGE_1_2].Update    = Stage1_2::Update;
    StageManager[STAGE_1_2].Draw      = Stage1_2::Draw;
    StageManager[STAGE_1_2].Free      = Stage1_2::Free;
    StageManager[STAGE_1_2].Unload    = Stage1_2::Unload;
                        
    StageManager[STAGE_1_3].Init      = Stage1_3::Init;
    StageManager[STAGE_1_3].Load      = Stage1_3::Load;
    StageManager[STAGE_1_3].Update    = Stage1_3::Update;
    StageManager[STAGE_1_3].Draw      = Stage1_3::Draw;
    StageManager[STAGE_1_3].Free      = Stage1_3::Free;
    StageManager[STAGE_1_3].Unload    = Stage1_3::Unload;
                        
    StageManager[STAGE_2_1].Init      = Stage2_1::Init;
    StageManager[STAGE_2_1].Load      = Stage2_1::Load;
    StageManager[STAGE_2_1].Update    = Stage2_1::Update;
    StageManager[STAGE_2_1].Draw      = Stage2_1::Draw;
    StageManager[STAGE_2_1].Free      = Stage2_1::Free;
    StageManager[STAGE_2_1].Unload    = Stage2_1::Unload;
                        
    StageManager[STAGE_2_2].Init      = Stage2_2::Init;
    StageManager[STAGE_2_2].Load      = Stage2_2::Load;
    StageManager[STAGE_2_2].Update    = Stage2_2::Update;
    StageManager[STAGE_2_2].Draw      = Stage2_2::Draw;
    StageManager[STAGE_2_2].Free      = Stage2_2::Free;
    StageManager[STAGE_2_2].Unload    = Stage2_2::Unload;
                        
    StageManager[STAGE_2_3].Init      = Stage2_3::Init;
    StageManager[STAGE_2_3].Load      = Stage2_3::Load;
    StageManager[STAGE_2_3].Update    = Stage2_3::Update;
    StageManager[STAGE_2_3].Draw      = Stage2_3::Draw;
    StageManager[STAGE_2_3].Free      = Stage2_3::Free;
    StageManager[STAGE_2_3].Unload    = Stage2_3::Unload;
                        
    StageManager[STAGE_3_1].Init      = Stage3_1::Init;
    StageManager[STAGE_3_1].Load      = Stage3_1::Load;
    StageManager[STAGE_3_1].Update    = Stage3_1::Update;
    StageManager[STAGE_3_1].Draw      = Stage3_1::Draw;
    StageManager[STAGE_3_1].Free      = Stage3_1::Free;
    StageManager[STAGE_3_1].Unload    = Stage3_1::Unload;
                        
    StageManager[STAGE_3_2].Init      = Stage3_2::Init;
    StageManager[STAGE_3_2].Load      = Stage3_2::Load;
    StageManager[STAGE_3_2].Update    = Stage3_2::Update;
    StageManager[STAGE_3_2].Draw      = Stage3_2::Draw;
    StageManager[STAGE_3_2].Free      = Stage3_2::Free;
    StageManager[STAGE_3_2].Unload    = Stage3_2::Unload;

        StageManager[STAGE_3_3].Init      = Stage3_3::Init;
        StageManager[STAGE_3_3].Load      = Stage3_3::Load;
        StageManager[STAGE_3_3].Update    = Stage3_3::Update;
        StageManager[STAGE_3_3].Draw      = Stage3_3::Draw;
        StageManager[STAGE_3_3].Free      = Stage3_3::Free;
        StageManager[STAGE_3_3].Unload    = Stage3_3::Unload;

    StageManager[SS_SCORE].Init       = Init_Score_Page;
    StageManager[SS_SCORE].Load       = Load_Score_Page;
    StageManager[SS_SCORE].Update     = Update_Score_Page;
    StageManager[SS_SCORE].Draw       = Render_Score_Page;
    StageManager[SS_SCORE].Free       = Free_Score_Page;
    StageManager[SS_SCORE].Unload     = Unload_Score_Page;

    s_previous    = STAGE_2_1;
    s_current     = STAGE_2_1;
    s_next        = STAGE_2_1;
    s_after_score = STAGE_2_1;

  }
  
  /**************************************************************************************
  //
  // Load the current stage
  //
  **************************************************************************************/
  void SM_Load(void)
  {
    if (s_current != SS_RESTART)
      StageManager[s_current].Load();
    else
      s_current = s_next = s_previous;
  }

  /**************************************************************************************
  //
  // Initialize the current stage
  //
  **************************************************************************************/
  void SM_Init(void)
  {
    StageManager[s_current].Init();
  }

  /**************************************************************************************
  //
  // Updates the current stage
  //
  **************************************************************************************/
  void SM_Update(float dt)
  {
    SubStage_Finished() ? SM_Unload(), SM_Free() : StageManager[s_current].Update(dt);
  }

  /**************************************************************************************
  //
  // Draw the current stage
  //
  **************************************************************************************/
  void SM_Draw(void)
  {
    StageManager[s_current].Draw();
  }

  /**************************************************************************************
  //
  // Unload the resources that were used in the current stage
  //
  **************************************************************************************/
  void SM_Unload(void)
  {
    if (s_next != SS_RESTART)
      StageManager[s_current].Unload();
  }

  /**************************************************************************************
  //
  // Free the resources that were used in the current stage before changing to next stage
  //
  **************************************************************************************/
  void SM_Free(void)
  {
    StageManager[s_current].Free();

    s_previous = s_current;
    s_current = s_next;

    if (GSM::next != GS_QUIT && s_next != SS_QUIT)
    {
      SM_Load();
      SM_Init();
    }
  }

  /**************************************************************************************
  //
  // set the next state 
  //
  **************************************************************************************/
  void Set_Next(STAGE_LIST next)
  {
    s_next = next;
  }

  /**************************************************************************************
  //
  // Get the next state
  //
  **************************************************************************************/
    STAGE_LIST Get_Next()
    {
        return s_next;
    }

    /**************************************************************************************
    //
    // set the next state
    //
    **************************************************************************************/
    void Set_Curr(STAGE_LIST next)
    {
      s_current = next;
    }

  /**************************************************************************************
  //
  // Get the current state
  //
  **************************************************************************************/
    STAGE_LIST Get_Curr()
    {
        return s_current;
    }

  /**************************************************************************************
  //
  // Set the next state after the score page
  //
  **************************************************************************************/
  void Set_After_Score(STAGE_LIST after_score)
  {
    s_after_score = after_score;
  }

  /**************************************************************************************
  //
  // return the state after_score is referring to
  //
  **************************************************************************************/
  STAGE_LIST Get_After_Score(void)
  {
    return s_after_score;
  }

  /**************************************************************************************
  //
  // checks if the current stage is complete
  //
  **************************************************************************************/
  bool SubStage_Finished(void)
  {
    return s_current != s_next;
  }


}
