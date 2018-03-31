/* Start Header ************************************************************************/
/*!
\file       GameStateManager.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Manages the game state

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "GameStateManager.h"
#include "StageManager.h"
#include "Input_Handler.h"
#include "AI_Test_Stage.h"

namespace GSM
{

	GAME_STATE previous = GS_LEVEL3_3;
	GAME_STATE current  = GS_LEVEL3_3;
	GAME_STATE next     = GS_LEVEL3_3;

	GameStateFunctions GSF[GS_QUIT] = { 0 };

	// initializes the array of function pointers for each state
	void GSM_Init(void)
	{
		GSF[GS_MAIN].Init         = Main_Menu::Init;
		GSF[GS_MAIN].Load         = Main_Menu::Load;
		GSF[GS_MAIN].Update       = Main_Menu::Update;
		GSF[GS_MAIN].Draw         = Main_Menu::Draw;
		GSF[GS_MAIN].Free         = Main_Menu::Free;
		GSF[GS_MAIN].Unload       = Main_Menu::Unload;

		GSF[GS_LEVEL1_1].Init	  = Test_Stage1_1::Init;
		GSF[GS_LEVEL1_1].Load	  = Test_Stage1_1::Load;
		GSF[GS_LEVEL1_1].Update   = Test_Stage1_1::Update;
		GSF[GS_LEVEL1_1].Draw	  = Test_Stage1_1::Draw;
		GSF[GS_LEVEL1_1].Free	  = Test_Stage1_1::Free;
		GSF[GS_LEVEL1_1].Unload   = Test_Stage1_1::Unload;

		GSF[GS_LEVEL1_2].Init     = Test_Stage1_2::Init;
		GSF[GS_LEVEL1_2].Load     = Test_Stage1_2::Load;
		GSF[GS_LEVEL1_2].Update   = Test_Stage1_2::Update;
		GSF[GS_LEVEL1_2].Draw     = Test_Stage1_2::Draw;
		GSF[GS_LEVEL1_2].Free     = Test_Stage1_2::Free;
		GSF[GS_LEVEL1_2].Unload   = Test_Stage1_2::Unload;
								  
		GSF[GS_LEVEL1_3].Init     = Test_Stage1_3::Init;
		GSF[GS_LEVEL1_3].Load     = Test_Stage1_3::Load;
		GSF[GS_LEVEL1_3].Update   = Test_Stage1_3::Update;
		GSF[GS_LEVEL1_3].Draw     = Test_Stage1_3::Draw;
		GSF[GS_LEVEL1_3].Free     = Test_Stage1_3::Free;
		GSF[GS_LEVEL1_3].Unload   = Test_Stage1_3::Unload;

		GSF[GS_LEVEL2_1].Init     = Stage2_1::Init;
		GSF[GS_LEVEL2_1].Load     = Stage2_1::Load;
		GSF[GS_LEVEL2_1].Update   = Stage2_1::Update;
		GSF[GS_LEVEL2_1].Draw     = Stage2_1::Draw;
		GSF[GS_LEVEL2_1].Free     = Stage2_1::Free;
		GSF[GS_LEVEL2_1].Unload   = Stage2_1::Unload;

		GSF[GS_LEVEL2_2].Init     = Stage2_2::Init;
		GSF[GS_LEVEL2_2].Load     = Stage2_2::Load;
		GSF[GS_LEVEL2_2].Update   = Stage2_2::Update;
		GSF[GS_LEVEL2_2].Draw     = Stage2_2::Draw;
		GSF[GS_LEVEL2_2].Free     = Stage2_2::Free;
		GSF[GS_LEVEL2_2].Unload   = Stage2_2::Unload;

		GSF[GS_LEVEL2_3].Init     = Stage2_3::Init;
		GSF[GS_LEVEL2_3].Load     = Stage2_3::Load;
		GSF[GS_LEVEL2_3].Update   = Stage2_3::Update;
		GSF[GS_LEVEL2_3].Draw     = Stage2_3::Draw;
		GSF[GS_LEVEL2_3].Free     = Stage2_3::Free;
		GSF[GS_LEVEL2_3].Unload   = Stage2_3::Unload;

		GSF[GS_LEVEL3_1].Init     = Stage3_1::Init;
		GSF[GS_LEVEL3_1].Load     = Stage3_1::Load;
		GSF[GS_LEVEL3_1].Update   = Stage3_1::Update;
		GSF[GS_LEVEL3_1].Draw     = Stage3_1::Draw;
		GSF[GS_LEVEL3_1].Free     = Stage3_1::Free;
		GSF[GS_LEVEL3_1].Unload   = Stage3_1::Unload;

		GSF[GS_LEVEL3_2].Init     = Stage3_2::Init;
		GSF[GS_LEVEL3_2].Load     = Stage3_2::Load;
		GSF[GS_LEVEL3_2].Update   = Stage3_2::Update;
		GSF[GS_LEVEL3_2].Draw     = Stage3_2::Draw;
		GSF[GS_LEVEL3_2].Free     = Stage3_2::Free;
		GSF[GS_LEVEL3_2].Unload   = Stage3_2::Unload;

		GSF[GS_LEVEL3_3].Init     = Stage3_3::Init;
		GSF[GS_LEVEL3_3].Load     = Stage3_3::Load;
		GSF[GS_LEVEL3_3].Update   = Stage3_3::Update;
		GSF[GS_LEVEL3_3].Draw     = Stage3_3::Draw;
		GSF[GS_LEVEL3_3].Free     = Stage3_3::Free;
		GSF[GS_LEVEL3_3].Unload   = Stage3_3::Unload;

		GSF[AI_TEST_STAGE].Init   = AI_Test_Stage::Init;
		GSF[AI_TEST_STAGE].Load   = AI_Test_Stage::Load;
		GSF[AI_TEST_STAGE].Update = AI_Test_Stage::Update;
		GSF[AI_TEST_STAGE].Draw   = AI_Test_Stage::Draw;
		GSF[AI_TEST_STAGE].Free   = AI_Test_Stage::Free;
		GSF[AI_TEST_STAGE].Unload = AI_Test_Stage::Unload;

        GSF[GS_LEVELS].Init       = SM::SM_Init;
        GSF[GS_LEVELS].Load       = SM::SM_Load;
        GSF[GS_LEVELS].Update     = SM::SM_Update;
        GSF[GS_LEVELS].Draw       = SM::SM_Draw;
        GSF[GS_LEVELS].Free       = SM::SM_Free;
        GSF[GS_LEVELS].Unload     = SM::SM_Unload;

        GSF[GS_CREDITS].Init      = Credits::Init;
        GSF[GS_CREDITS].Load      = Credits::Load;
        GSF[GS_CREDITS].Update    = Credits::Update;
        GSF[GS_CREDITS].Draw      = Credits::Draw;
        GSF[GS_CREDITS].Free      = Credits::Free;
        GSF[GS_CREDITS].Unload    = Credits::Unload;

		//GSF[GS_LEVEL_IMPORT].Init   = Test_Stage_Import::Init;
		//GSF[GS_LEVEL_IMPORT].Load   = Test_Stage_Import::Load;
		//GSF[GS_LEVEL_IMPORT].Update = Test_Stage_Import::Update;
		//GSF[GS_LEVEL_IMPORT].Draw   = Test_Stage_Import::Draw;
		//GSF[GS_LEVEL_IMPORT].Free   = Test_Stage_Import::Free;
		//GSF[GS_LEVEL_IMPORT].Unload = Test_Stage_Import::Unload;
		
		SM::StageManagerInit();
		Attack_Sprite_Init();
		AI_Sprite_Init();
		Effects_Init();
	}

	void Init_and_Load(void)
	{
		if (current == GS_RESTART)
		{
			//current and next state set to the previous state
			current = next = previous;
		}

		else if (current != GS_RESTART)
		{
			GSF[current].Load();
		}


		GSF[current].Init();

	}

	void Update_and_Draw(float dt)
	{
        if (Input::Quit_Triggered())
        {
            next = GS_QUIT;
        }

		GSF[current].Update(dt);


		GSF[current].Draw();

	}

	void Cleanup(void)
	{

		if (next != GS_RESTART)
		{

			GSF[current].Unload();
		}

		GSF[current].Free();

		previous = current;
		current = next;
	}



	bool IsGameRunning(void)
	{
		return (current != GS_QUIT);
	}

	void GSM_Cleanup(void)
	{
		Effects_Cleanup();
	}
}
