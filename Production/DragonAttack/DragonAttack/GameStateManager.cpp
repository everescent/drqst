#include "GameStateManager.h"

typedef void(*void_fp_void)(void);
typedef void(*void_fp_float)(float deltaTime);

struct GameStateFunctions
{
	void_fp_void  GameStateInit;
	void_fp_float GameStateUpdate;
	void_fp_void  GameStateDraw;
	void_fp_void  GameStateCleanup;
};

void GameStateManagerInit(void)
{

}