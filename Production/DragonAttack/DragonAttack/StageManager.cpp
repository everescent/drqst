#include "StageManager.h"
#include "Stage3_3.h"
#include "Score_Page.h"


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

        StageManager[STAGE_3_3].Init      = Stage3_3::Init;
        StageManager[STAGE_3_3].Load      = Stage3_3::Load;
        StageManager[STAGE_3_3].Update    = Stage3_3::Update;
        StageManager[STAGE_3_3].Draw      = Stage3_3::Draw;
        StageManager[STAGE_3_3].Free      = Stage3_3::Free;
        StageManager[STAGE_3_3].Unload    = Stage3_3::Unload;

		StageManager[SS_SCORE].Init       = Init_Score_Page;
		StageManager[SS_SCORE].Load       = Load_Score_Page;
		StageManager[SS_SCORE].Update     = Print_Score_Page;
		StageManager[SS_SCORE].Draw       = Render_Score_Page;
		StageManager[SS_SCORE].Free       = Free_Score_Page;
		StageManager[SS_SCORE].Unload     = Unload_Score_Page;

		s_previous = SS_SCORE;
		s_current  = SS_SCORE;
		s_next     = SS_SCORE;

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

    STAGE_LIST Get_Next()
    {
        return s_next;
    }

    STAGE_LIST Get_Curr()
    {
        return s_current;
    }

	bool SubStage_Finished(void)
	{
		return s_current != s_next;
	}

	bool Stage_Finished(void)
	{
		return s_current == SS_QUIT;
	}
}
