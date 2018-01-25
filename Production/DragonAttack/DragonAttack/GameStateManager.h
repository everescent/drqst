#pragma once
#include "GameStateList.h"
#include "Main_Menu.h"

namespace GSM
{
	// holds the previous, current and next state
	extern GAME_STATE previous, current, next;

	void GSM_Init(void);

	void Init_and_Load(void);

	void Update_and_Draw(float dt);

	void Cleanup(void);

	bool IsGameRunning(void);

}