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

namespace GSM
{

	GAME_STATE previous = GS_MAIN;
	GAME_STATE current  = GS_MAIN;
	GAME_STATE next     = GS_MAIN;

	GameStateFunctions GSF[GS_QUIT] = { 0 };

	// initializes the array of function pointers for each state
	void GSM_Init(void)
	{
		GSF[GS_MAIN].Init     = Main_Menu::Init;
		GSF[GS_MAIN].Load     = Main_Menu::Load;
		GSF[GS_MAIN].Update   = Main_Menu::Update;
		GSF[GS_MAIN].Draw     = Main_Menu::Draw;
		GSF[GS_MAIN].Free     = Main_Menu::Free;
		GSF[GS_MAIN].Unload   = Main_Menu::Unload;

		GSF[GS_LEVEL1].Init	  = Test_Stage1::Init;
		GSF[GS_LEVEL1].Load	  = Test_Stage1::Load;
		GSF[GS_LEVEL1].Update = Test_Stage1::Update;
		GSF[GS_LEVEL1].Draw	  = Test_Stage1::Draw;
		GSF[GS_LEVEL1].Free	  = Test_Stage1::Free;
		GSF[GS_LEVEL1].Unload = Test_Stage1::Unload;
		
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
