#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameStateManager.h"
#include "Collision.h"
#include "resource.h"
#include <utility>
#include <iostream>

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  UNREFERENCED_PARAMETER(instanceH);
  UNREFERENCED_PARAMETER(prevInstanceH);
  UNREFERENCED_PARAMETER(command_line);
  UNREFERENCED_PARAMETER(show);

  const int R_WIDTH{ 1280 }; //Screen width
  const int R_HEIGHT{ 720 }; //Screen height

  float dt = 0.0f;       // delta time
  float timePast = 0.0f; // time past in game
  #if defined(DEBUG) | defined(_DEBUG)
    AESysInit(instanceH, show, R_WIDTH, R_HEIGHT, true, 60, NULL);
  #else
    AESysInit(instanceH, show, R_WIDTH, R_HEIGHT, false, 60, NULL);
  #endif
  AESysSetWindowTitle("Dragon Attack");
  AESysReset();
  GSM::GSM_Init();

  //HICON icon = LoadIcon(instanceH, MAKEINTRESOURCE(IDI_ICON1));
 // SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));

  #if defined(DEBUG) | defined(_DEBUG)
  	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  #endif

  while (GSM::IsGameRunning())
 {
	GSM::Init_and_Load();

    while (GSM::current == GSM::next)
    {
      AESysFrameStart();
      AEInputUpdate();
      GSM::Update_and_Draw(dt);
      AESysFrameEnd();

	  dt = (f32)AEFrameRateControllerGetFrameTime();
	  timePast += dt;
    }



    GSM::Cleanup();
  }

  GSM::GSM_Cleanup();
  AESysExit();
 // DestroyIcon(icon);

  return 0;
}
