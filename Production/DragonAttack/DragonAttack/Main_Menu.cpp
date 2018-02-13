#include "Main_Menu.h"
#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "Collision.h"
#include "GameStateManager.h"
#include "Dragon.h"
#include "Merlin.h"
#include "Floor.h"
#include "Platform.h"
#include <utility>


namespace
{
  //Col_Comp t_col{ 1.0, 1.0, Point };

  //class testCol : public GameObject {
  //public:
  //	void Pos() {/*Do something*/ }
  //	void Update() {/*Do something*/ }
  //	testCol()
  //		:colli{ 0.0f,0.0f, 50, Circle }, GameObject{ S_CreateSquare(50.0f,1.0f,1.0f,"floor.jpg"), t_col }
  //	{}

  //	Col_Comp colli;
  //};
  //static float aX = -300.0f;
  //static float aY = 0.0f;
  //static float bX = 300.0f;
  //static float bY = 0.0f;

  //testCol *a = nullptr;
  //testCol *b = nullptr;
  Dragon *player;
  Merlin *enemy;
  Sprite *BG;
  Transform *M_BG;
  Floor *floor1;
  Platform *plat1;
}

namespace Main_Menu
{
  void Init(void)
  {

  }

  void Load(void)
  {
    //a = new testCol;
    //b = new testCol;
    player = new Dragon{};
    enemy = new Merlin{};
    BG = new Sprite{ CreateBG(1.0f, 1.0f, 1.0f, "floor.jpg") };
    M_BG = new Transform{};
	floor1 = new Floor{};
	plat1 = new Platform{};
  }

  void Update(float dt)
  {
    //UNREFERENCED_PARAMETER(dt);
    //aX += 2.0f;
    //bX -= 2.0f;
    //a->Transform_.SetTranslate(aX, aY);
    //b->Transform_.SetTranslate(bX, bY);
    //a->colli.Update_Col_Pos(aX, aY);
    //b->colli.Update_Col_Pos(bX, bY);
    //if (a->colli.Circle_Circle(a->colli, b->colli))
    //	GSM::current = GS_QUIT;
	plat1->SetActive(true);
	floor1->SetActive(true);
    player->SetActive(true);
    player->Update(dt);
    enemy->SetActive(true);
    enemy->Update(*player);
	plat1->Update(*player);
	floor1->Update(*player);
  }

  void Draw(void)
  {
    // a->Render();
    //b->Render();
    //CamFollow(player->Transform_, Cam_Offset_X, Cam_Offset_Y, player->GetFacing());
    CamStatic();
    BG->Render_Object(*M_BG);
    player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    player->Render();
    enemy->Render();
	plat1->Render();
	floor1->Render();
  }

  void Free(void)
  {
    //delete a;
    //delete b;
    delete BG;
    delete player;
    delete enemy;
  }

  void Unload(void)
  {

  }
}