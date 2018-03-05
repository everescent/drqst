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

	static int** MapData;
	int Map_Width;
	int Map_Height;
	std::vector<Platform> platforms;
	std::vector<Floor> floors;
	std::vector<Grunt> grunts;
	std::vector<Wall> walls;
	std::vector<Barrier> barriers;
	std::vector<Scarecrow> scarecrows;
	std::vector<PickUp> powerups;
	//std::vector<Archer> archers;

	Tower *archerTower;
	Archer *archer1, *archer2;
	PickUp *coin1, *coin2, *coin3;
	PickUp *power1, *power2;
}

namespace Test_Stage1_1
{
	void Load(void)
	{
		BG = new Sprite{ CreateBG(5.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 5.0f) };
		M_BG = new Transform{};
		player = new Dragon{};
		archerTower = new Tower{ 4800.0f, 0.0f };

		archer1 = new Archer{ 5000.0f, 200.0f };
		archer2 = new Archer{ 6200.0f, -220.0f };

		coin1 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 1500.0f, -210.0f };

		coin2 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 3550.0f, -350.0f };

		coin3 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 6300.0f, -220.0f };

		power1 = new PickUp{ S_CreateSquare(50.0f, "Textures/spd.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			SPD, 0.0f, -220.0f };

		power2 = new PickUp{ S_CreateSquare(50.0f, "Textures/hp.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			HP, 5300.0f, -100.0f };

		if (!Import_MapData("level2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		/*std::cout << "Width:" << Map_Width << std::endl;
		std::cout << "Height:" << Map_Height << std::endl;
		PrintRetrievedInformation(MapData, Map_Width, Map_Height);*/
	}

	void Init(void)
	{
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
				if (MapData[y][x] == OBJ_FLOOR)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					floors.push_back(Floor{ Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_GRUNT)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					grunts.push_back(Grunt{ Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_SC)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					scarecrows.push_back(Scarecrow{ Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_WALL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					walls.push_back(Wall{ Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_BARRIER)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					barriers.push_back(Barrier{ Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_SPD)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					powerups.push_back(PickUp{ S_CreateSquare(50.0f, "Textures/spd.png",1.0f),
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						HP, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_HP)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					powerups.push_back(PickUp{ S_CreateSquare(50.0f, "Textures/hp.png",1.0f),
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						HP, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_COIN)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					powerups.push_back(PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						HP, Convert_X(f_x) , Convert_Y(f_y) });
				}
				/*if (MapData[y][x] == OBJ_ARCHER)
				{
				float f_x = (float)x;
				float f_y = (float)y;
				archers.push_back(Archer{ Convert_X(f_x) , Convert_Y(f_y) });
				}*/
			}
		}
	}

	void Update(float dt)
	{
		for (Platform& elem : platforms)
		{
			elem.Update(*player, dt);
		}
		for (Floor& elem : floors)
		{
			for (Grunt& elem2 : grunts)
			{
				elem.Update(elem2, dt);
			}
			for (Scarecrow& elem3 : scarecrows)
			{
				elem.Update(elem3, dt);
			}
			elem.Update(*player, dt);
		}
		for (Wall& elem : walls)
		{
			elem.Update(*player, dt);
		}
		for (Barrier& elem : barriers)
		{
			elem.Update(*player, dt);
		}
		for (Scarecrow& elem : scarecrows)
		{
			elem.Update(*player, dt);
		}
		for (Grunt& elem : grunts)
		{
			elem.Update(*player, dt);
		}
		/*for (Archer& elem : archers)
		{
		elem.Update(*player, dt);
		}*/
		archerTower->Update(*player, dt);
		archer1->Update(*player, dt);
		archer2->Update(*player, dt);
		player->Update(*player, dt);
		coin1->Update(*player, dt);
		coin2->Update(*player, dt);
		coin3->Update(*player, dt);
		power1->Update(*player, dt);
		power2->Update(*player, dt);

		//std::cout << player->Get_Charge() << std::endl;
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 200, 120, player->GetFacing());

		BG->Render_Object(*M_BG);

		archer1->Render();
		archer1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		archer2->Render();
		archer2->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		coin1->Render();
		coin1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		coin2->Render();
		coin2->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		coin3->Render();
		coin3->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		power1->Render();
		power1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		power2->Render();
		power2->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		archerTower->Render();
		archerTower->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		for (Platform& elem : platforms)
		{
			elem.Render();
		}
		for (Floor& elem : floors)
		{
			elem.Render();
		}
		/*for (Wall& elem : walls)
		{
		elem.Render();
		}*/
		for (Scarecrow& elem : scarecrows)
		{
			elem.Render();
			elem.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}
		for (Barrier& elem : barriers)
		{
			elem.Render();
			elem.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}
		for (Grunt& elem : grunts)
		{
			elem.Render();
			elem.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}
		/*for (Archer& elem : archers)
		{
		elem.Render();
		elem.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}*/
		player->Render();
		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete archerTower;
		delete archer1;
		delete archer2;
		delete coin1;
		delete coin2;
		delete coin3;
		delete power1;
		delete power2;
	}

	void Unload(void)
	{

	}
}