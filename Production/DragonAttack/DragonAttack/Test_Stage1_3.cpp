#include "Test_Stage1_3.h"

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	Audio_Engine* Audio;

	int** MapData;
	int Map_Width;
	int Map_Height;

	std::vector<Floor> floors;
	std::vector<Wall> walls;

	LevelChangePlatform *next;
	std::vector<Characters*> c;
}

namespace Test_Stage1_3
{
	void Load(void)
	{
		BG = new Sprite{ CreateBG(5.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 5.0f) };
		M_BG = new Transform{};
		player = new Dragon{};
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Lancelot_BGM.mp3"); } };

		if (!Import_MapData("level1-3.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		next = new LevelChangePlatform{ 500.0f,  -300.0f };
		c.push_back(Create_Boss_AI(LANCELOT));
	}


	void Init(void)
	{
		Audio->Play(0);
		Audio->SetLoop(0, 1);

		c[0]->SetActive(true);
		player->SetActive(true);

		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_FLOOR)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					floors.push_back(Floor{ Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_WALL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					walls.push_back(Wall{ Convert_X(f_x) , Convert_Y(f_y) });
				}
			}
		}
	}



	void Update(float dt)
	{
		Audio->Update();
		
		c[0]->Update(*player, dt);
		if (c[0]->Get_HP() <= 0)
		{
			next->Update(*player, dt);
		}

		for (Floor& elem : floors)
		{
			for (size_t i = 0; i < c.size(); ++i)
			{
				elem.Update(*(c[i]), dt);
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

		//next->Update(*player, dt);
		player->Update(*player, dt);

		//std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 200, 120, player->GetFacing());

		BG->Render_Object(*M_BG);

		for (Floor& elem : floors)
		{
			elem.Render();
		}

		/*for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Render();
		}*/
		c[0]->Render();
		if (c[0]->Get_HP() <= 0)
		{
			next->Render();
		}

		player->Render();
		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete Audio;
		delete next;

		floors.clear();
		walls.clear();
		c.clear();

		for (int y = 0; y < Map_Height; ++y)
		{
			delete[] MapData[y];
		}
		delete[] MapData;
	}

	void Unload(void)
	{

	}
}