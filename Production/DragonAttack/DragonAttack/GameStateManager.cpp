/* Start Header ************************************************************************/
/*!
\file       GameStateManager.cpp
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
#include "GameStateManager.h"
#include "Input_Handler.h"

namespace GSM
{
	// defining a pointer to a void funtion that takes in void
	typedef void(*void_fp_void)(void);

	// defining a pointer to a void funtion that takes in a float
	typedef void(*void_fp_float)(float deltaTime);

	GAME_STATE previous = GS_MAIN;
	GAME_STATE current = GS_MAIN;
	GAME_STATE next = GS_MAIN;

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

	GameStateFunctions GSF[GS_RESTART] = { 0 };

	// initializes the array of function pointers for each state
	void GSM_Init(void)
	{
		GSF[GS_MAIN].Init   = Main_Menu::Init;
		GSF[GS_MAIN].Load   = Main_Menu::Load;
		GSF[GS_MAIN].Update = Main_Menu::Update;
		GSF[GS_MAIN].Draw   = Main_Menu::Draw;
		GSF[GS_MAIN].Free   = Main_Menu::Free;
		GSF[GS_MAIN].Unload = Main_Menu::Unload;

		GSF[GS_QUIT].Init   = nullptr;
		GSF[GS_QUIT].Load   = nullptr;
		GSF[GS_QUIT].Update = nullptr;
		GSF[GS_QUIT].Draw   = nullptr;
		GSF[GS_QUIT].Free   = nullptr;
		GSF[GS_QUIT].Unload = nullptr;

		GSF[current].Init();
	}

	void Init_and_Load(void)
	{
		if (current == GS_RESTART)
		{
			//current and next state set to the previous state
			current = next = previous;
		}

		else if (GSF[current].Load != nullptr)
		{
			GSF[current].Load();
		}

		if (GSF[current].Init != nullptr)
		{
			GSF[current].Init();
		}
	}

	void Update_and_Draw(float dt)
	{
		Get_User_Input();

		
		if (GSF[current].Update != nullptr)
		{
			GSF[current].Update(dt);
		}

		if (GSF[current].Draw != nullptr)
		{
			GSF[current].Draw();
		}
	}

	void Cleanup(void)
	{
		if (GSF[current].Free != nullptr)
		{
			GSF[current].Free();
		}

		if (next != GS_RESTART)
		{
			if (GSF[current].Unload != nullptr)
			{
				GSF[current].Unload();
			}
		}
	}

	bool IsGameRunning(void)
	{
		return (current != GS_QUIT);
	}
}
