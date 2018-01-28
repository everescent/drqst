#include "AEEngine.h"
#include "Objects.h"
#include "Transform.h"
#include "Create_Object.h"
#include "Camera.h"
#include "GameStateManager.h"

const int R_WIDTH { 1280 }; //Screen width
const int R_HEIGHT{ 720  }; //Screen height

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  UNREFERENCED_PARAMETER(instanceH);
  UNREFERENCED_PARAMETER(prevInstanceH);
  UNREFERENCED_PARAMETER(command_line);
  UNREFERENCED_PARAMETER(show);

  AESysInit(instanceH, show, R_WIDTH, R_HEIGHT, true, 60, NULL);
  AESysSetWindowTitle("My New Demo!");
  AESysReset();
  GSM::GSM_Init();
  Object Platform1(CreateRectangle(PLAT_WIDTH * 2, PLAT_HEIGHT), "floor.jpg", PLAT_WIDTH * 2, PLAT_HEIGHT);
  Transform Move;
  Move.SetTranslate(1000.0f, 1000.0f).SetScale(2.0f, 2.0f);
  while (GSM::IsGameRunning())
 {
    while (GSM::current == GSM::next)
    {
      AESysFrameStart();
      CamFollow(Move, 80.0f, 150.0f);
      Platform1.Render_Object(Move);
      GSM::Update_and_Draw(2.0);
      AESysFrameEnd();
    }
  }
  AESysExit();
  return 0;
}
