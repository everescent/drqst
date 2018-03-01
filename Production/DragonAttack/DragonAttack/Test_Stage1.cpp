#include "Test_Stage1.h"
#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "Collision.h"
#include "GameStateManager.h"
#include "Dragon.h"
#include "Floor.h"
#include "Wall.h"
#include "Platform.h"
#include "Barrier.h"
#include "AI_Data_Factory.h"
#include <utility>
#include <iostream>

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	
	Grunt *scarecrow1;
	Grunt *scarecrow2;
	Barrier *fence1;
	Grunt *soldier1 ,*soldier2;
	Grunt *archer1;
	//Grunt *archerTower;
	Platform *plat1, *plat2, *plat3;

	Floor *floor1, *floor2, *platformingFloor, *exitFloor;
	Wall *LeftWall, *platformingWall1, *platformingWall2, *exitWall;

	/*std::vector<Characters*> c;
	char num_of_mob = 1;*/
}

namespace Test_Stage1
{
	void Init(void)
	{
		/*for(char i = 0; i < num_of_mob; ++i)
		c[i]->SetActive(true);*/

		player->SetActive(true);
	}

	void Load(void)
	{
		BG     = new Sprite    { CreateBG(1.0f, "../../Illustrations/Environment/Cobblestone.png") };
		M_BG   = new Transform { };
		player = new Dragon    { };

		// Enemies
		scarecrow1       = new Grunt { 750.0f, -190.0f };
		scarecrow2       = new Grunt { 950.0f, -190.0f };
		soldier1         = new Grunt { 1900.0f, -190.0f };
		soldier2         = new Grunt { 2000.0f, -190.0f };
		archer1          = new Grunt { 4500.0f, -70.0f };

		// Fence
		fence1           = new Barrier{ 1400.0f, -170.0f };

		// Floating Platforms
		plat1            = new Platform { 2600.0f, -150.0f }; //ok
		plat2            = new Platform { 2800.0f, -20.0f }; //ok
		plat3            = new Platform { 3000.0f, 110.0f }; //ok
				
		// Walls
		LeftWall         = new Wall { -620.0f, -160.0f }; //ok
		platformingWall1 = new Wall { 2430.0f, -475.0f }; //ok
		platformingWall2 = new Wall { 3350.0f, -475.0f };
		exitWall         = new Wall { 5000.0f, -70.0f }; //ok

		// Floors
		floor1           = new Floor { 0.0f, -350.0f }; //ok
		floor2           = new Floor { 1200.0f, -350.0f }; //ok
		platformingFloor = new Floor { 2500.0f, -500.0f }; //ok
		exitFloor        = new Floor { 4575.0f, -230.0f }; //ok

		//c.push_back(Create_Boss_AI(KING_ARTHUR));
	}
						 

	void Update(float dt)
	{
		scarecrow1->Update(*player, dt);
		scarecrow2->Update(*player, dt);
		soldier1->Update(*player, dt);
		soldier2->Update(*player, dt);
		archer1->Update(*player, dt);

		fence1->Update(*player, dt);

		plat1->Update(*player, dt);
		plat2->Update(*player, dt);
		plat3->Update(*player, dt);

		LeftWall->Update(*player, dt);
		platformingWall1->Update(*player, dt);
		platformingWall2->Update(*player, dt);
		exitWall->Update(*player, dt);

		floor1->Update(*player, dt);
		floor2->Update(*player, dt);
		platformingFloor->Update(*player, dt);
		exitFloor->Update(*player, dt);

		player->Update(*player, dt);
		std::cout << (int)player->PosX <<", "<< (int)player->PosY << std::endl;
		/*for (char i = 0; i < num_of_mob; ++i)
			c[i]->Update(*player, dt);*/
			
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 20, 120, player->GetFacing());
		/*if (AEInputCheckCurr(AEVK_LSHIFT))
		{
			CamFollow(player->Transform_, Cam_Offset_X, Cam_Offset_Y, player->GetFacing());
		}
		else
		{
			CamStatic();
		}*/

		BG->Render_Object(*M_BG);


		scarecrow1->Render();
		scarecrow1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		scarecrow2->Render();
		scarecrow2->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		soldier1->Render();
		soldier1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		soldier2->Render();
		soldier2->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		archer1->Render();
		archer1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		fence1->Render();
		fence1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		plat1->Render();
		plat2->Render();
		plat3->Render();

		LeftWall->Render();
		platformingWall1->Render();
		platformingWall2->Render();
		exitWall->Render();

		floor1->Render();
		floor2->Render();
		platformingFloor->Render();
		exitFloor->Render();

		player->Render();
		//player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		/*for (char i = 0; i < num_of_mob; ++i)
			c[i]->Render();*/
	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		// Enemies
		delete scarecrow1;
		delete scarecrow2;
		delete soldier1;
		delete soldier2;
		delete archer1;

		// Static Objs
		delete fence1;

		delete plat1;
		delete plat2;
		delete plat3;

		delete LeftWall;
		delete platformingWall1;
		delete platformingWall2;
		delete exitWall;

		delete floor1;
		delete floor2;
		delete platformingFloor;
		delete exitFloor;

		//c.clear();
	}

	void Unload(void)
	{

	}
}