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
#include "Grunt.h"
#include "Platform.h"
#include "Wall.h"
#include <utility>


namespace
{
  Dragon *player;
  //Merlin *enemy;
  Grunt *grunt;
  Sprite *BG;
  Transform *M_BG;
  Floor *floor1;
  Platform *plat1, *plat2, *plat3, *plat4, *plat5, *plat6;
  Wall *Lwall1, *Lwall2;
}

namespace Main_Menu
{
  void Init(void)
  {

  }

  void Load(void)
  {
    BG     = new Sprite    { CreateBG(1.0f, "floor.jpg") };
    M_BG   = new Transform { };
	floor1 = new Floor     { 0.0f, -350.0f };
	plat1  = new Platform  { 0.0f, -150.0f};
	plat2  = new Platform  { 200.0f, -20.0f };
	plat3  = new Platform  { -200.0f, -20.0f };
	plat4  = new Platform  { 0.0f, 120.0f };
	plat5  = new Platform  { -350.0f, 120.0f };
	plat6  = new Platform  { 350.0f, 120.0f };
	
	Lwall1 = new Wall      { -620.0f, -160.0f };
	Lwall2 = new Wall      { -620.0f, 160.0f };

	player = new Dragon    { };
	//enemy  = new Merlin    { };
	grunt  = new Grunt     { 400.0f, -160.0f };
  }

  void Update(float dt)
  {
	floor1->Update(*player, dt);
	plat1->Update (*player, dt);
	plat2->Update (*player, dt);
	plat3->Update (*player, dt);
	plat4->Update (*player, dt);
	plat5->Update (*player, dt);
	plat6->Update (*player, dt);

	Lwall1->Update(*player);
	Lwall2->Update(*player);

	player->SetActive(true);
    player->Update(dt);
    //enemy->SetActive(true);
    //enemy->Update(*player);
	grunt->Update(dt, *player);

	if (AEInputCheckCurr(AEVK_0))
	{

	}

  }

  void Draw(void)
  {

	if (AEInputCheckCurr(AEVK_LSHIFT)) 
	{
		CamFollow(player->Transform_, Cam_Offset_X, Cam_Offset_Y, player->GetFacing());
	}
	else
	{
		CamStatic();
	}

	BG->Render_Object(*M_BG);
	floor1->Render();
	plat1->Render();
	plat2->Render();
	plat3->Render();
	plat4->Render();
	plat5->Render();
	plat6->Render();
    
    player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	player->Render();
    //enemy->Render();
	grunt->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	grunt->Render();
  }

  void Free(void)
  {
    delete BG;
	delete plat1, plat2, plat3, plat4, plat5, plat6;
	delete Lwall1, Lwall2;
	delete floor1;

	//delete enemy;
	delete player;
	delete grunt;
  }

  void Unload(void)
  {

  }
}