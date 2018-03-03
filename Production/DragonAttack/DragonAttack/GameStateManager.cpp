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

<<<<<<< HEAD
	GAME_STATE previous = GS_MAIN;
	GAME_STATE current  = GS_MAIN;
	GAME_STATE next     = GS_MAIN;
=======
	GAME_STATE previous = AI_TEST_STAGE;
	GAME_STATE current  = AI_TEST_STAGE;
	GAME_STATE next     = AI_TEST_STAGE;
>>>>>>> 00a38363ae4e6e1f1ed1da47c8a32c2fcf7aa322

	GameStateFunctions GSF[GS_QUIT] = { 0 };

	// initializes the array of function pointers for each state
	void GSM_Init(void)
	{
<<<<<<< HEAD
		GSF[GS_MAIN].Init     = EngineProof::Init;
		GSF[GS_MAIN].Load     = EngineProof::Load;
		GSF[GS_MAIN].Update   = EngineProof::Update;
		GSF[GS_MAIN].Draw     = EngineProof::Draw;
		GSF[GS_MAIN].Free     = EngineProof::Free;
		GSF[GS_MAIN].Unload   = EngineProof::Unload;
=======
		//GSF[GS_MAIN].Init     = Main_Menu::Init;
		//GSF[GS_MAIN].Load     = Main_Menu::Load;
		//GSF[GS_MAIN].Update   = Main_Menu::Update;
		//GSF[GS_MAIN].Draw     = Main_Menu::Draw;
		//GSF[GS_MAIN].Free     = Main_Menu::Free;
		//GSF[GS_MAIN].Unload   = Main_Menu::Unload;
>>>>>>> 00a38363ae4e6e1f1ed1da47c8a32c2fcf7aa322

		GSF[GS_LEVEL1].Init	  = Test_Stage1_1::Init;
		GSF[GS_LEVEL1].Load	  = Test_Stage1_1::Load;
		GSF[GS_LEVEL1].Update = Test_Stage1_1::Update;
		GSF[GS_LEVEL1].Draw	  = Test_Stage1_1::Draw;
		GSF[GS_LEVEL1].Free	  = Test_Stage1_1::Free;
		GSF[GS_LEVEL1].Unload = Test_Stage1_1::Unload;

		GSF[AI_TEST_STAGE].Init   = AI_Test_Stage::Init;
		GSF[AI_TEST_STAGE].Load   = AI_Test_Stage::Load;
		GSF[AI_TEST_STAGE].Update = AI_Test_Stage::Update;
		GSF[AI_TEST_STAGE].Draw   = AI_Test_Stage::Draw;
		GSF[AI_TEST_STAGE].Free   = AI_Test_Stage::Free;
		GSF[AI_TEST_STAGE].Unload = AI_Test_Stage::Unload;

		/*
		GSF[GS_LEVEL2].Init   = Test_Stage1_2::Init;
		GSF[GS_LEVEL2].Load   = Test_Stage1_2::Load;
		GSF[GS_LEVEL2].Update = Test_Stage1_2::Update;
		GSF[GS_LEVEL2].Draw   = Test_Stage1_2::Draw;
		GSF[GS_LEVEL2].Free   = Test_Stage1_2::Free;
		GSF[GS_LEVEL2].Unload = Test_Stage1_2::Unload;
		*/

		//GSF[GS_LEVEL_IMPORT].Init   = Level_Import::Init;
		//GSF[GS_LEVEL_IMPORT].Load   = Level_Import::Load;
		//GSF[GS_LEVEL_IMPORT].Update = Level_Import::Update;
		//GSF[GS_LEVEL_IMPORT].Draw   = Level_Import::Draw;
		//GSF[GS_LEVEL_IMPORT].Free   = Level_Import::Free;
		//GSF[GS_LEVEL_IMPORT].Unload = Level_Import::Unload;

		/*
		GSF[GS_LEVEL2].Init   = Test_Stage1_2::Init;
		GSF[GS_LEVEL2].Load   = Test_Stage1_2::Load;
		GSF[GS_LEVEL2].Update = Test_Stage1_2::Update;
		GSF[GS_LEVEL2].Draw   = Test_Stage1_2::Draw;
		GSF[GS_LEVEL2].Free   = Test_Stage1_2::Free;
		GSF[GS_LEVEL2].Unload = Test_Stage1_2::Unload;
		*/

		//GSF[GS_LEVEL_IMPORT].Init   = Level_Import::Init;
		//GSF[GS_LEVEL_IMPORT].Load   = Level_Import::Load;
		//GSF[GS_LEVEL_IMPORT].Update = Level_Import::Update;
		//GSF[GS_LEVEL_IMPORT].Draw   = Level_Import::Draw;
		//GSF[GS_LEVEL_IMPORT].Free   = Level_Import::Free;
		//GSF[GS_LEVEL_IMPORT].Unload = Level_Import::Unload;

		GSF[GS_MAIN].Init		= Main_Menu::Init;
		GSF[GS_MAIN].Load		= Main_Menu::Load;
		GSF[GS_MAIN].Update = Main_Menu::Update;
		GSF[GS_MAIN].Draw		= Main_Menu::Draw;
		GSF[GS_MAIN].Free		= Main_Menu::Free;
		GSF[GS_MAIN].Unload = Main_Menu::Unload;
		
		SM::StageManagerInit();
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
	    Input::Get_User_Input();


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
}
