#include "Test_Stage1_2.h"
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
	//std::vector<Grunt> grunts;
	std::vector<Wall> walls;
	std::vector<Barrier> barriers;
	std::vector<Scarecrow> scarecrows;
	//std::vector<PickUp> powerups;

	Tower *archerTower;
	PickUp *coin1, *coin2, *coin3, *hp;
	Platform *up1, *up2;
	//PickUp *power1, *power2;
	std::vector<Characters*> c;
}

namespace Test_Stage1_2
{
	void Load(void)
	{
		BG = new Sprite{ CreateBG(5.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 5.0f) };
		M_BG = new Transform{};
		player = new Dragon{};

		if (!Import_MapData("level1-2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		// Tower
		archerTower = new Tower{ 7000.0f, 170.0f };

		up1 = new Platform{ 6000.0f, -30.0f };
		up2 = new Platform{ 6000.0f, -120.0f };

		coin1 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 2080.0f , -145.0f };

		coin2 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 4200.0f , -325.0f };

		coin3 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 5450.0f , 200.0f };

		hp = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			HP, 5450.0f , 200.0f };

		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 727.0f , 105.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 1895.0f , -165.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 2335.0f , 195.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 4500.0f , -345.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 4700.0f , -345.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 4900.0f , -345.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 5550.0f , 195.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 5300.0f , 195.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 6450.0f , 195.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 7000.0f, 400.0f }));
	}


	void Init(void)
	{
		for(size_t i = 0; i < c.size(); ++i)
			c[i]->SetActive(true);

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
				/*if (MapData[y][x] == OBJ_GRUNT)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					grunts.push_back(Grunt{ Convert_X(f_x) , Convert_Y(f_y) });
				}*/
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
				/*if (MapData[y][x] == OBJ_SPD)
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
				}*/
			}
		}

	}



	void Update(float dt)
	{
		for (size_t i = 0; i < c.size(); ++i)
			c[i]->Update(*player, dt);

		for (Platform& elem : platforms)
		{
			elem.Update(*player, dt);
		}
		for (Floor& elem : floors)
		{
			for (size_t i = 0; i < c.size(); ++i)
			{
				elem.Update(*(c[i]), dt);
			}

			for (Scarecrow& elem3 : scarecrows)
			{
				elem.Update(elem3, dt);
			}
			elem.Update(*player, dt);
		}
		for (Wall& elem : walls)
		{
			for (size_t i = 0; i < c.size(); ++i)
			{
				elem.Update(*(c[i]), dt);
			}


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
		/*for (PickUp& elem : powerups)
		{
			elem.Update(*player, dt);
		}*/

		coin1->Update(*player, dt);
		coin2->Update(*player, dt);
		coin3->Update(*player, dt);
		archerTower->Update(*player, dt);
		up1->Update(*player, dt);
		up2->Update(*player, dt);
		player->Update(*player, dt);

		std::cout << (int)player->PosX <<", "<< (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 200, 120, player->GetFacing());

		BG->Render_Object(*M_BG);

		archerTower->Render();
		archerTower->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		up1->Render();
		up2->Render();

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
	/*	for (PickUp& elem : powerups)
		{
			elem.Render();
			elem.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}*/

		player->Render();
		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Render();
			c[i]->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}
		coin1->Render();
		coin1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		coin2->Render();
		coin2->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		coin3->Render();
		coin3->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		// Static Objs
		delete archerTower;
		delete up1;
		delete up2;
		delete coin1;
		delete coin2;
		delete coin3;
		c.clear();
	}

	void Unload(void)
	{

	}
}