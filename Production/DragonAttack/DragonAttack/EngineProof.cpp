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
#include "Animation.h"
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
  Animation *ani;
}

namespace EngineProof
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

  /*Animation class example: 
    In the instantiation below, I am passing in a lambda function.
    This lambda function initializes the Animation_State array to the desired ranges of each state.
    []  -> This means the lamda can only use variables declared within the scope of the lambda
    [=] -> This means the lambda can use local variables within the scope (by copy)
    [&] -> This means the lambda can use local variables within the scope (by referemce)
    [Variable Name]  -> This means the lambda only uses variable by copy and nothing else
    [&Variable Name] -> This means the lambda only uses variable by reference and nothing else
    ()  -> Put in parameters as you would in a normal function
    -> T ---> This specifies the return type is T, if not compiler will guess

    You can also pass in a normal function if you do not understand how to use lambdas
  */
  ani = new Animation{ 3, 12.0f, 12.0f, 10.0f, 10.0f, [](std::vector <Range> &A_State) -> void{
      float x = 0.0f, y = 0.0f;
      float x1 = 0.0f, y1 = 0.0f;
      for (int i = 0; i < 3; ++i, x += 10.0f, y += 10.0f, x1 += 12.0f, y1 += 12.0f)
      {
        A_State.push_back(Range{ x, y, x1, y1 });
      }
    }};
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

	Lwall1->Update(*player, dt);
	Lwall2->Update(*player, dt);

	player->SetActive(true);
    player->Update(*player, dt);
    //enemy->SetActive(true);
    //enemy->Update(*player);
	grunt->Update(*player, dt);

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