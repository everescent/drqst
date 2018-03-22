#include "Stage2_1.h"

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	Audio_Engine* Audio;
	UI* ui;
	int** MapData;
	int Map_Width;
	int Map_Height;

	std::vector<Platform> platforms;
	std::vector<Floor> floors;
	std::vector<Wall> walls;
	std::vector<Barrier> barriers;

	LevelChangePlatform *next;
	PickUp *coin1, *coin2, *coin3, *hp, *invul;
	std::vector<Characters*> c;

	Wall *w1, *w2, *w3, *w4, *w5, *w6, *w7, *w8, *w9, *w10, *w11, *w12, *w13, *w14, *w15, *w16, *w17, *w18, *w19, *w20;
	Sprite* COIN_SPRITE;//pickups					 							   
	Sprite* HP_SPRITE;
	Sprite* DMG_SPRITE;
	Sprite* SPD_SPRITE;
	Sprite* BARRIER_SPRITE;//objs												   
	Sprite* WALL_SPRITE;
	Sprite* PLAT_SPRITE;
	Sprite* LCPLAT_SPRITE;
	Sprite* FLOOR_SPRITE;
	Sprite* TOWER_SPRITE;
	Sprite* SIGN_SPRITE;
}

namespace Stage2_1
{
	void Load(void)
	{
		COIN_SPRITE = new Sprite{ S_CreateSquare(50.0f, "Textures/coin.png", 1.0f) };
		HP_SPRITE = new Sprite{ S_CreateSquare(50.0f, "Textures/hp.png", 1.0f) };
		DMG_SPRITE = new Sprite{ S_CreateSquare(50.0f, "Textures/Fireball.png", 1.0f) };
		SPD_SPRITE = new Sprite{ S_CreateSquare(50.0f, "Textures/spd.png", 1.0f) };
		BARRIER_SPRITE = new Sprite{ S_CreateSquare(130.0f, ".//Textures/box.png") };
		WALL_SPRITE = new Sprite{ S_CreateRectangle(50.0f, 50.0f, ".//Textures/download.jpg") };
		PLAT_SPRITE = new Sprite{ CreatePlatform(1.0f, 1.0f, "Textures/Cobblestone.png") };
		LCPLAT_SPRITE = new Sprite{ CreatePlatform(2.0f, 3.0f, ".//Textures/Win_Platform.png") };
		FLOOR_SPRITE = new Sprite{ CreateFloor(1.0f, "Textures/Cobblestone.png", 1.0f, 1.0f) };
		TOWER_SPRITE = new Sprite{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png") };
		SIGN_SPRITE = new Sprite{ S_CreateSquare(70.0f, ".//Textures/sign.png") };

		BG = new Sprite{ CreateBG(22.0f, 2.0f, "Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON));
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_1_BGM.mp3"); } };
		ui = new UI(player);
		if (!Import_MapData("level2-1.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }
		/*
		next = new LevelChangePlatform{ LCPLAT_SPRITE, 7500.0f,  240.0f };
		*/
		w1  = new Wall{ WALL_SPRITE, 2240.0f, -690.0f };
		w2  = new Wall{ WALL_SPRITE, 2240.0f, -630.0f };
		w3  = new Wall{ WALL_SPRITE, 2240.0f, -570.0f };
		 
		w4  = new Wall{ WALL_SPRITE, 2650.0f, -560.0f };
		w5  = new Wall{ WALL_SPRITE, 2650.0f, -630.0f };
		    
		w6  = new Wall{ WALL_SPRITE, 4955.0f, -330.0f };
		w7  = new Wall{ WALL_SPRITE, 4955.0f, -400.0f };
		w8  = new Wall{ WALL_SPRITE, 4955.0f, -440.0f };
		 
		w9  = new Wall{ WALL_SPRITE, 6460.0f, -170.0f };
		w10 = new Wall{ WALL_SPRITE, 6460.0f, -230.0f };
		w11 = new Wall{ WALL_SPRITE, 6460.0f, -280.0f };
		w12 = new Wall{ WALL_SPRITE, 6460.0f, -330.0f };
		w13 = new Wall{ WALL_SPRITE, 6460.0f, -400.0f };
		w14 = new Wall{ WALL_SPRITE, 6460.0f, -440.0f };
		 
		w15 = new Wall{ WALL_SPRITE, 6460.0f,  130.0f };
		w16 = new Wall{ WALL_SPRITE, 6460.0f,   80.0f };
		w17 = new Wall{ WALL_SPRITE, 6460.0f,   30.0f };
		w18 = new Wall{ WALL_SPRITE, 6460.0f,  -20.0f };
		w19 = new Wall{ WALL_SPRITE, 6460.0f,  -70.0f };
		w20 = new Wall{ WALL_SPRITE, 6460.0f, -120.0f };

		/*
		coin1 = new PickUp{ COIN_SPRITE,
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 2080.0f , -680.0f };

		coin2 = new PickUp{ COIN_SPRITE,
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 2800.0f , -600.0f };

		coin3 = new PickUp{ COIN_SPRITE,
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 5750.0f , 200.0f };

		hp = new PickUp{ HP_SPRITE,
			Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			HP, 4300.0f , 60.0f };
*/
		// Enemy placements (12)
		// Grunts
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{  2615.0f ,  -1065.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 5875.0f ,  -255.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 5095.0f ,  -1155.0f }));
		c.push_back(Create_Basic_AI(GRUNT, AEVec2{ 5000.0f ,  -3405.0f }));
		// Archers
		//c.push_back(Create_Basic_AI(ARCHER, AEVec2{ -85.0f ,  -75.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 500.0f ,  -885.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 2930.0f ,  -2325.0f }));
		// Mages
		c.push_back(Create_Basic_AI(MAGE, AEVec2{ 1000.0f ,  -615.0f }));
		c.push_back(Create_Basic_AI(MAGE, AEVec2{ 3100.0f ,  -75.0f }));
		c.push_back(Create_Basic_AI(MAGE, AEVec2{ 4780.0f ,  -2325.0f }));
		// Knights
		//c.push_back(Create_Basic_AI(KNIGHT, AEVec2{ 7200.0f ,  300.0f }));
		
	}


	void Init(void)
	{
		Audio->Play(0);
		Audio->SetLoop(0, 1);

		for (size_t i = 0; i < c.size(); ++i)
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
					platforms.push_back(Platform{ PLAT_SPRITE, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_FLOOR)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					floors.push_back(Floor{ FLOOR_SPRITE,Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_WALL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					walls.push_back(Wall{ WALL_SPRITE,Convert_X(f_x) , Convert_Y(f_y) });
				}
				/*if (MapData[y][x] == OBJ_BARRIER)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					barriers.push_back(Barrier{ BARRIER_SPRITE,Convert_X(f_x) , Convert_Y(f_y) });
				}*/
			}
		}
	}



	void Update(float dt)
	{
		Audio->Update();
		
		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Update(*player, dt);
			w1->Update(*(c[i]), dt);
			w2->Update(*(c[i]), dt);
			w3->Update(*(c[i]), dt);
			w4->Update(*(c[i]), dt);
			w5->Update(*(c[i]), dt);
			w6->Update(*(c[i]), dt);
			w7->Update(*(c[i]), dt);
			w8->Update(*(c[i]), dt);
			w9->Update(*(c[i]), dt);
			w10->Update(*(c[i]), dt);
			w11->Update(*(c[i]), dt);
			w12->Update(*(c[i]), dt);
			w13->Update(*(c[i]), dt);
			w14->Update(*(c[i]), dt);
			w15->Update(*(c[i]), dt);
			w16->Update(*(c[i]), dt);
			w17->Update(*(c[i]), dt);
			w18->Update(*(c[i]), dt);
			w19->Update(*(c[i]), dt);
			w20->Update(*(c[i]), dt);
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

		/*coin1->Update(*player, dt);
		coin2->Update(*player, dt);
		coin3->Update(*player, dt);
		next->Update(*player, dt);*/
		player->Update(*player, dt);
		CamFollow(player->Transform_, 200, 120, player->GetFacing());
		ui->UI_Update(player);

		w1->Update(*player, dt);
		w2->Update(*player, dt);
		w3->Update(*player, dt);
		w4->Update(*player, dt);
		w5->Update(*player, dt);

		w6->Update(*player, dt);
		w7->Update(*player, dt);
		w8->Update(*player, dt);
		w9->Update(*player, dt);
		w10->Update(*player, dt);
		w11->Update(*player, dt);
		w12->Update(*player, dt);
		w13->Update(*player, dt);
		w14->Update(*player, dt);

		w15->Update(*player, dt);
		w16->Update(*player, dt);
		w17->Update(*player, dt);
		w18->Update(*player, dt);
		w19->Update(*player, dt);
		w20->Update(*player, dt);

		std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		BG->Render_Object(*M_BG);

		for (Platform& elem : platforms)
		{
			elem.Render();
		}
		for (Floor& elem : floors)
		{
			elem.Render();
		}
		for (Barrier& elem : barriers)
		{
			elem.Render();
		}
		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Render();
		}
		/*coin1->Render();
		coin2->Render();
		coin3->Render();
		hp->Render();
		next->Render();
		*/
		w1->Render();
		w2->Render();
		w3->Render();
		w4->Render();
		w5->Render();
		w6 ->Render();
		w7->Render();
		w8->Render();
		w9->Render();
		w10->Render();
		w11->Render();
		w12->Render();
		w13->Render();
		w14->Render();
		w15->Render();
		w16->Render();
		w17->Render();
		w18->Render();
		w19->Render();
		w20->Render();
		
		player->Render();
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		ui->Render();

	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete Audio;

		delete w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12, w13, w14, w15, w16, w17, w18, w19, w20;
/*
		delete coin1, coin2, coin3, hp;
		delete next;*/
		delete ui;

		//delete COIN_SPRITE;//pickups
		//delete HP_SPRITE;
		//delete DMG_SPRITE;
		//delete SPD_SPRITE;
		//delete BARRIER_SPRITE;//objs
		//delete WALL_SPRITE;
		//delete PLAT_SPRITE;
		//delete LCPLAT_SPRITE;
		//delete FLOOR_SPRITE;
		//delete TOWER_SPRITE;
		//delete SIGN_SPRITE;

		platforms.clear();
		floors.clear();
		walls.clear();
		barriers.clear();
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