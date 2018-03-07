#include "Test_Stage1.h"

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	Audio_Engine* Audio;

	int** MapData;
	int Map_Width;
	int Map_Height;

	std::vector<Platform> platforms;
	std::vector<Floor> floors;
	std::vector<Wall> walls;
	std::vector<Barrier> barriers;
	std::vector<Scarecrow> scarecrows;
	std::vector<PickUp> powerups;

	LevelChangePlatform *next;
	Tower *archerTower;
	PickUp *coin1, *coin2, *coin3;
	PickUp *power1, *power2;
	Scarecrow *sc1, *sc2;
	Barrier *box1;

	std::vector<Characters*> c;
}

namespace Test_Stage1_1
{
	void Load(void)
	{
		BG = new Sprite{ CreateBG(15.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};
		player = new Dragon{};
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_1_BGM.mp3"); } };
		archerTower = new Tower{ 4800.0f, 0.0f };

		next = new LevelChangePlatform{ 6550.0f, -120.0f };

		coin1 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 1500.0f, -210.0f };

		coin2 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 3550.0f, -350.0f };

		coin3 = new PickUp{ S_CreateSquare(50.0f, "Textures/coin.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 6300.0f, -220.0f };

		//power1 = new PickUp{ S_CreateSquare(50.0f, "Textures/spd.png",1.0f),
		//	Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
		//	SPD, 0.0f, -220.0f };

		power2 = new PickUp{ S_CreateSquare(50.0f, "Textures/hp.png",1.0f),
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			HP, 5300.0f, -100.0f };

		sc1 = new Scarecrow{ 700.0f, -240.0f };
		sc2 = new Scarecrow{ 1000.0f, -240.0f };
		box1 = new Barrier{ 1500.0f, -235.0f };

		if (!Import_MapData("level1-1.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		c.push_back(Create_Basic_AI(GRUNT,  AEVec2  { 2320.0f , -180.0f }));
		c.push_back(Create_Basic_AI(GRUNT,  AEVec2  { 2480.0f , -180.0f }));
		c.push_back(Create_Basic_AI(GRUNT,  AEVec2  { 4000.0f ,  -90.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2  { 4750.0f ,  140.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2  { 6200.0f , -220.0f }));
	}

	void Init(void)
	{
		player->SetActive(true);
		Audio->Play(0);
		Audio->SetLoop(0, 1);
		for (size_t i = 0; i < c.size(); ++i)
			c[i]->SetActive(true);

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
			}
		}
	}

	void Update(float dt)
	{
		Audio->Update();

		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Update(*player, dt);
			archerTower->Update(*(c[i]), dt);
		}

		if (!(box1->IsActive()))
		{
			coin1->Update(*player, dt);
		}

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

			elem.Update(*sc1, dt);
			elem.Update(*sc2, dt);
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
			elem.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}
		for (Scarecrow& elem : scarecrows)
		{
			elem.Update(*player, dt);
			elem.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}
		
		archerTower->Update(*player, dt);
		player->Update(*player, dt);
		sc1->Update(*player, dt);
		sc2->Update(*player, dt);
		box1->Update(*player, dt);
		coin2->Update(*player, dt);
		coin3->Update(*player, dt);
		//power1->Update(*player, dt);
		power2->Update(*player, dt);
		next->Update(*player, dt);

		//std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 200, 120, player->GetFacing());

		BG->Render_Object(*M_BG);


		if (!(box1->IsActive()))
		{
			coin1->Render();
		}
		coin2->Render();
		coin3->Render();
		//power1->Render();
		power2->Render();

		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Render();
		}
		archerTower->Render();
		for (Platform& elem : platforms)
		{
			elem.Render();
		}
		for (Floor& elem : floors)
		{
			elem.Render();
		}
		for (Scarecrow& elem : scarecrows)
		{
			elem.Render();
		}
		for (Barrier& elem : barriers)
		{
			elem.Render();
		}

		sc1->Render();
		sc2->Render();
		box1->Render();
		player->Render();
		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		next->Render();
	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete Audio;

		delete sc1;
		delete sc2;
		delete box1;
		delete archerTower;
		delete coin1;
		delete coin2;
		delete coin3;
		//delete power1;
		delete power2;
		delete next;

		platforms.clear();
		floors.clear();
		walls.clear();
		barriers.clear();
		scarecrows.clear();
		powerups.clear();
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