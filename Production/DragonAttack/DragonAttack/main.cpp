#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameStateManager.h"
#include "Collision.h"
#include <utility>

//class testCol : public GameObject {
//public:
//	void Pos() {/*Do something*/ }
//	void Update() {/*Do something*/ }
//	Col_Comp &getCol() { return colli; }
//	testCol()
//		:colli{ 0.0f, 0.0f, 10, Circle }, GameObject{ S_CreateSquare(50.0f,1.0f,1.0f,"floor.jpg") }
//	{}
//	Col_Comp colli;
//};
//static float aX = -300.0f;
//static float aY = 0.0f;
//static float bX = 300.0f;
//static float bY = 0.0f;

//testCol a;
//testCol* a;

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  UNREFERENCED_PARAMETER(instanceH);
  UNREFERENCED_PARAMETER(prevInstanceH);
  UNREFERENCED_PARAMETER(command_line);
  UNREFERENCED_PARAMETER(show);

  const int R_WIDTH{ 1280 }; //Screen width
  const int R_HEIGHT{ 720 }; //Screen height
  //testCol b;
  AESysInit(instanceH, show, R_WIDTH, R_HEIGHT, true, 60, NULL);
  AESysSetWindowTitle("My New Demo!");
  AESysReset();
  GSM::GSM_Init();
  //a = new testCol;

  while (GSM::IsGameRunning())
 {
	  GSM::Init_and_Load();
	  while (GSM::current == GSM::next)
    {
	  AESysFrameStart();
	  //++aX;
	  //--bX;
	  //a.Transform_.SetTranslate(aX, aY);
	  //b.Transform_.SetTranslate(bX, bY);
	  //a.Render();
	  //b.Render();
	  //if (a.colli.Rect_Rect(a.getCol(), b.getCol()))
		 // return 0;
      GSM::Update_and_Draw(2.0);
      AESysFrameEnd();
    }
	GSM::Cleanup();
  }
  AESysExit();
  return 0;
}
