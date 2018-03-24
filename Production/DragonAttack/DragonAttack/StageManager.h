#pragma once
#include "GameStateList.h"



namespace SM
{
	void StageManagerInit(void);

	void SM_Load(void);
	void SM_Init   (void);
	void SM_Update (float dt);
	void SM_Draw   (void);
	void SM_Unload (void);
	void SM_Free   (void);

	void       Set_Next(STAGE_LIST);
    STAGE_LIST Get_Next();
    STAGE_LIST Get_Curr();
	bool       SubStage_Finished(void);
	bool       Stage_Finished(void);
}