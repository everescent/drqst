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
	/*std::vector<Characters*> c;
	char num_of_mob = 1;*/
}

namespace Test_Stage1_2
{
	void Load(void)
	{
		BG = new Sprite{ CreateBG(5.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 5.0f) };
		M_BG = new Transform{};
		player = new Dragon{};

		if (!Import_MapData("level1-2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		archer1 = new Archer{ 4500.0f, 230.0f };

		// Tower
		archerTower = new Tower{ 4500.0f, 0.0f };
		//c.push_back(Create_Boss_AI(KING_ARTHUR));
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

		archer1->Update(*player, dt);
		archerTower->Update(*player, dt);
		player->Update(*player, dt);

		//std::cout << (int)player->PosX <<", "<< (int)player->PosY << std::endl;

		/*for (char i = 0; i < num_of_mob; ++i)
		c[i]->Update(*player, dt);*/
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 200, 120, player->GetFacing());

		BG->Render_Object(*M_BG);

		archer1->Render();
		archer1->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
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

		/*for (char i = 0; i < num_of_mob; ++i)
		c[i]->Render();*/
	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete archer1;
		// Static Objs
		delete archerTower;
		//c.clear();
	}

	void Unload(void)
	{

	}
}