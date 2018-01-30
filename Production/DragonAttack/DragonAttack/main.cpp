#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameStateManager.h"
#include <utility>

const int R_WIDTH { 1280 }; //Screen width
const int R_HEIGHT{ 720  }; //Screen height

class testGO : public GameObject
{
public:
  testGO(Sprite&& tmp)
    :GameObject{ std::move(tmp) }
  {}
  void Update(){/*Something*/}
  void Pos(){/*Something*/}
};


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
  testGO test(CreatePlatform(2.0f, 1.0f, 1.0f, 1.0f, "floor.jpg"));
  //Sprite Platform1(CreatePlatform(2.0f, 1.0f, 1.0f, 1.0f, "floor.jpg"));
  Sprite Floor1(CreateRectangle(FLOOR_WIDTH, FLOOR_HEIGHT), "floor.jpg", FLOOR_WIDTH, FLOOR_HEIGHT);
  Transform M_Floor;
  Transform Move;
  while (GSM::IsGameRunning())
 {
    while (GSM::current == GSM::next)
    {
      static float x = 0.0f;
      static float y = 0.0f;
      AESysFrameStart();
      if (AEInputCheckCurr(AEVK_LEFT))
      {
        x -= 5.0f;
      }
      if (AEInputCheckCurr(AEVK_RIGHT))
      {
        x += 5.0f;
      }
      if (AEInputCheckCurr(AEVK_UP))
      {
        y += 5.0f;
      }
      if (AEInputCheckCurr(AEVK_DOWN))
      {
        y -= 5.0f;
      }
      test.Transform_.SetTranslate(x, y);
      CamFollow(test.Transform_, 80.0f, 150.0f);
      Floor1.Render_Object(M_Floor);
      test.Render();
      //Platform1.Render_Object(Move);
      GSM::Update_and_Draw(2.0);
      AESysFrameEnd();
    }
  }
  AESysExit();
  return 0;
}
