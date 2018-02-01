#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameStateManager.h"
#include "Collision.h"
#include <utility>

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  UNREFERENCED_PARAMETER(instanceH);
  UNREFERENCED_PARAMETER(prevInstanceH);
  UNREFERENCED_PARAMETER(command_line);
  UNREFERENCED_PARAMETER(show);

  const int R_WIDTH{ 1280 }; //Screen width
  const int R_HEIGHT{ 720 }; //Screen height

  AESysInit(instanceH, show, R_WIDTH, R_HEIGHT, true, 60, NULL);
  AESysSetWindowTitle("My New Demo!");
  AESysReset();
  GSM::GSM_Init();

  while (GSM::IsGameRunning())
 {
    GSM::Init_and_Load();
    while (GSM::current == GSM::next)
    {
      AESysFrameStart();
      GSM::Update_and_Draw(2.0);
      AESysFrameEnd();
    }
    GSM::Cleanup();
  }
  AESysExit();
  return 0;
}
