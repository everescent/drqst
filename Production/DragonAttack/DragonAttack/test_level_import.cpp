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

	Tower *archerTower;
	Grunt *archer;
}

namespace Test_Stage_Import
{
	void Load(void)
	{
		BG = new Sprite{ CreateBG(5.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 5.0f) };
		M_BG = new Transform{};
		player = new Dragon{};
		archerTower = new Tower{ 4300.0f, 0.0f };
		archer = new Grunt{ 4300.0f, 200.0f };

		if (!Import_MapData("level2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		std::cout << "Width:" << Map_Width << std::endl;
		std::cout << "Height:" << Map_Height << std::endl;
		PrintRetrievedInformation(MapData, Map_Width, Map_Height);
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
		archerTower->Update(*player, dt);
		archer->Update(*player, dt);
		player->Update(*player, dt);

		std::cout << player->Get_Charge() << std::endl;
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 20, 120, player->GetFacing());

		BG->Render_Object(*M_BG);

		archer->Render();
		archer->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

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
		player->Render();
		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete archerTower;
		delete archer;
	}

	void Unload(void)
	{

	}
}