#include "StageManager.h"


static STAGE_LIST s_previous, s_current, s_next;

namespace SM
{
	GameStateFunctions StageManager[SS_QUIT] = { nullptr };

	void StageManagerInit()
	{
		StageManager[SUB_STAGE1].Init     = nullptr;
		StageManager[SUB_STAGE1].Load     = nullptr;
		StageManager[SUB_STAGE1].Update   = nullptr;
		StageManager[SUB_STAGE1].Draw     = nullptr;
		StageManager[SUB_STAGE1].Free     = nullptr;
		StageManager[SUB_STAGE1].Unload   = nullptr;



		s_previous = SUB_STAGE1;
		s_current  = SUB_STAGE1;
		s_next     = SUB_STAGE1;
	}
	
	void SM_Load(void)
	{
		if (s_current != SS_RESTART)
			StageManager[s_current].Load();
	}

	void SM_Init(void)
	{
		StageManager[s_current].Init();
	}

	void SM_Update(float dt)
	{
		StageManager[s_current].Update(dt);
	}

	void SM_Draw(void)
	{
		StageManager[s_current].Draw();
	}

	void SM_Unload(void)
	{
		if (s_next != SS_RESTART)
			StageManager[s_current].Unload();
	}

	void SM_Free(void)
	{
		StageManager[s_current].Free();

		s_previous = s_current;
		s_current = s_next;
	}

	void Set_Next(STAGE_LIST next)
	{
		s_next = next;
	}

	bool SubStage_Finished(void)
	{
		return s_current == s_next;
	}

	bool Stage_Finished(void)
	{
		return s_current == SS_QUIT;
	}
}
