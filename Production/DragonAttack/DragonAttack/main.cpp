#include "AEEngine.h"
#include "Objects.h"
#include "GameStateManager.h"


#define R_WIDTH 1280
#define R_HEIGHT 720

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  UNREFERENCED_PARAMETER(instanceH);
  UNREFERENCED_PARAMETER(prevInstanceH);
  UNREFERENCED_PARAMETER(command_line);
  UNREFERENCED_PARAMETER(show);

  AESysInit(instanceH, show, R_WIDTH, R_HEIGHT, false, 60, NULL);
  AESysSetWindowTitle("My New Demo!");
  AESysReset();
  GSM::GSM_Init();
  Object Platform1(CreatePlatform(4.0f, 2.0f, 2.0f, 2.0f), "floor.png");
  while (GSM::IsGameRunning())
 {
    AESysFrameStart();
    RenderObject(Platform1);
    AESysFrameEnd();
  }
  AESysExit();
  return 0;
}
