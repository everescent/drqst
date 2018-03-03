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
#include "Tower.h"
#include "Scarecrow.h"
#include "AI_Data_Factory.h"
#include "Level_Import.h"
#include <utility>
#include <iostream>

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	
	Scarecrow *scarecrow1, *scarecrow2;
	Barrier *box1, *box_health;
	Grunt *soldier1 ,*soldier2;
	Grunt *archer1;
	Tower *archerTower;
	Platform *plat1, *plat2, *plat3;

	Floor *floor1, *floor2, *platformingFloor, *exitFloor, *testFloor;
	Wall *LeftWall, *platformingWall1, *platformingWall2, *platformingWall3, *exitWall;

	static int** MapData;
	int Map_Width;
	int Map_Height;
	std::vector<Platform> platforms;

	/*std::vector<Characters*> c;
	char num_of_mob = 1;*/
}

namespace Test_Stage1_1
{
//	void Load(void)
//	{
//		if (!Import_MapData("level2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }
//
//		std::cout << "Width:" << Map_Width << std::endl;
//		std::cout << "Height:" << Map_Height << std::endl;
//		PrintRetrievedInformation(MapData, Map_Width, Map_Height);
//
//
//		BG = new Sprite{ CreateBG(1.0f, "../../Illustrations/Environment/Cobblestone.png") };
//		M_BG = new Transform{};
//		player = new Dragon{};
//
//		// Enemies
//		scarecrow1 = new Scarecrow{ 750.0f, -190.0f };
//		scarecrow2 = new Scarecrow{ 1050.0f, -190.0f };
//		soldier1 = new Grunt{ 1900.0f, -190.0f };
//		soldier2 = new Grunt{ 2000.0f, -190.0f };
//		archer1 = new Grunt{ 4500.0f, 230.0f };
//
//		// Tower
//		archerTower = new Tower{ 4500.0f, 0.0f };
//
//		// Fence
//		box1 = new Barrier{ 1400.0f, -170.0f };
//		box_health = new Barrier{ 4900.0f, -50.0f };
//
//		// Floating Platforms
//		plat1 = new Platform{ 2600.0f, -150.0f }; //ok
//		plat2 = new Platform{ 2800.0f, -20.0f }; //ok
//		plat3 = new Platform{ 3000.0f, 110.0f }; //ok
//
//																						 // Walls
//		LeftWall = new Wall{ -620.0f, -160.0f }; //ok
//		platformingWall1 = new Wall{ 2430.0f, -475.0f }; //ok
//		platformingWall2 = new Wall2{ 3335.0f, -475.0f }; //ok
//		exitWall = new Wall2{ 5200.0f, -70.0f }; //ok
//
//																						 // Floors
//		floor1 = new Floor{ 0.0f, -350.0f }; //ok
//		floor2 = new Floor{ 1200.0f, -350.0f }; //ok
//		platformingFloor = new Floor{ 2500.0f, -500.0f }; //ok
//		exitFloor = new Floor{ 4650.0f, -230.0f }; //ok
//
//																							 //c.push_back(Create_Boss_AI(KING_ARTHUR));
//=======
	void Load(void)
	{
		BG = new Sprite { CreateBG(5.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 5.0f)};
		M_BG   = new Transform { };
		player = new Dragon    { };

		// Enemies
		scarecrow1       = new Scarecrow {  750.0f, -190.0f };
		scarecrow2       = new Scarecrow { 1050.0f, -190.0f };

		soldier1         = new Grunt { 1900.0f, -190.0f };
		soldier2         = new Grunt { 2000.0f, -190.0f };
		archer1          = new Grunt { 4500.0f,  230.0f };

		// Tower
		archerTower      = new Tower { 4500.0f, 0.0f };

		// Fence
		box1             = new Barrier { 1400.0f, -170.0f };
		box_health       = new Barrier { 4900.0f,  -50.0f };

		// Floating Platforms
		plat1            = new Platform { 2600.0f, -150.0f }; //ok
		plat2            = new Platform { 2850.0f,  -20.0f }; //ok
		plat3            = new Platform { 3050.0f,  110.0f }; //ok
				
		// Walls
		LeftWall         = new Wall  { -620.0f, -160.0f }; //ok
		platformingWall1 = new Wall  { 2430.0f, -475.0f }; //ok
		platformingWall2 = new Wall { 3360.0f, -475.0f }; //ok
		platformingWall3 = new Wall { 3360.0f, -405.0f }; //ok
		exitWall         = new Wall { 5200.0f, -70.0f  }; //ok

		// Floors
		floor1           = new Floor {    0.0f, -350.0f }; //ok
		floor2           = new Floor { 1200.0f, -350.0f }; //ok
		platformingFloor = new Floor { 2500.0f, -500.0f }; //ok
		exitFloor        = new Floor { 4600.0f, -230.0f }; //ok
		testFloor        = new Floor { 4600.0f, -330.0f }; //ok

	}
	
	void Init(void)
	{
		/*for(char i = 0; i < num_of_mob; ++i)
		c[i]->SetActive(true);*/

		player->SetActive(true);

		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_PLATFORM)
							 {
								 float f_x = (float)x;
								 float f_y = (float)y;
								 platforms.push_back(Platform{ Convert_X(f_x) , Convert_Y(f_y) });
							 }
			}
		}
	}

	
						 

	void Update(float dt)
	{
		for (Platform& elem : platforms)
					{
						elem.Update(*player, dt);
					}


		scarecrow1->Update(*player, dt);
		scarecrow2->Update(*player, dt);
		soldier1->Update(*player, dt);
		soldier2->Update(*player, dt);
		archer1->Update(*player, dt);

		archerTower->Update(*player, dt);
		box1->Update(*player, dt);
		box_health->Update(*player, dt);

		plat1->Update(*player, dt);
		plat2->Update(*player, dt);
		plat3->Update(*player, dt);

		LeftWall->Update(*player, dt);
		platformingWall1->Update(*player, dt);
		platformingWall2->Update(*player, dt);
		platformingWall3->Update(*player, dt);
		exitWall->Update(*player, dt);

		floor1->Update(*player, dt);
		floor2->Update(*player, dt);
		platformingFloor->Update(*player, dt);
		exitFloor->Update(*player, dt);
		testFloor->Update(*player, dt);

		player->Update(*player, dt);
		//std::cout << (int)player->PosX <<", "<< (int)player->PosY << std::endl;
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

		archerTower->Render();
		archerTower->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		box1->Render();
		box1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		box_health->Render();
		box_health->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		plat1->Render();
		plat2->Render();
		plat3->Render();

		//LeftWall->Render();
		//platformingWall1->Render();
		//platformingWall2->Render();
		//platformingWall3->Render();
		//exitWall->Render();

		floor1->Render();
		floor2->Render();
		platformingFloor->Render();
		exitFloor->Render();
		testFloor->Render();

		player->Render();
		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		for (Platform& elem : platforms)
					{
						elem.Render();
					}

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
		delete archerTower;
		delete box1;
		delete box_health;

		delete plat1;
		delete plat2;
		delete plat3;

		delete LeftWall;
		delete platformingWall1;
		delete platformingWall2;
		delete platformingWall3;
		delete exitWall;
		
		delete testFloor;
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