#include "Test_Stage1_2.h"

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
	//std::vector<Scarecrow> scarecrows;

	LevelChangePlatform *next;
	//Tower *archerTower;
	PickUp *coin1, *coin2, *coin3, *hp, *invul;
	Platform *up1, *up2;
	//PickUp *power1, *power2;
	std::vector<Characters*> c;

	Wall *w1, *w2, *w3, *w4, *w5, *w6, *w7, *w8, *w9, *w10, *w11, *w12, *w13, *w14, *w15, *w16, *w17, *w18;
}

namespace Test_Stage1_2
{
	void Load(void)
	{
		BG = new Sprite{ CreateBG(22.0f, 2.0f, "../../Illustrations/BG/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};
		player = new Dragon{};
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_1_BGM.mp3"); } };
		ui = new UI(player);
		if (!Import_MapData("level1-2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		//archerTower = new Tower        { 7000.0f,  170.0f };
		up1 = new Platform             { 6000.0f,  -30.0f };
		up2 = new Platform             { 6000.0f, -120.0f };
		next = new LevelChangePlatform { 7500.0f,  150.0f };

		w1  = new Wall{ 260.0f, -110.0f };
		w2  = new Wall{ 430.0f, -20.0f };
		w3  = new Wall{ 1095.0f, -20.0f };
		w4  = new Wall{ 1260.0f, -110.0f };
		w5  = new Wall{ 1410.0f,-200.0f };
		w6  = new Wall{ 2240.0f,-165.0f };//?
		w7  = new Wall{ 2030.0f,70.0f };
		w8  = new Wall{ 2610.0f,70.0f };
		w9  = new Wall{ 2760.0f,-30.0f };
		w10 = new Wall{ 2930.0f,-120.0f };
		w11 = new Wall{ 3090.0f,-210.0f };
		w12 = new Wall{ 3820.0f,-385.0f };//
		w13 = new Wall{ 3820.0f,-290.0f };
		w14 = new Wall{ 5380.0f,-385.0f };
		w15 = new Wall{ 5470.0f,-295.0f };
		w16 = new Wall{ 6230.0f,-165.0f };
		w17 = new Wall{ 6230.0f,-100.0f };
		w18 = new Wall{ 6230.0f,0.0f };

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

		c.push_back(Create_Basic_AI(GRUNT , AEVec2{  727.0f ,  105.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 1895.0f , -165.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 2335.0f ,  195.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 4500.0f , -345.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 4700.0f , -345.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 4900.0f , -345.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 5500.0f ,  195.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 5800.0f ,  195.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 6450.0f ,  195.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 6950.0f ,  300.0f }));
	}


	void Init(void)
	{
		Audio->Play(0);
		Audio->SetLoop(0, 1);

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
		Audio->Update();

		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Update(*player, dt);
			w1-> Update(*(c[i]), dt);
			w2-> Update(*(c[i]), dt);
			w3-> Update(*(c[i]), dt);
			w4-> Update(*(c[i]), dt);
			w5-> Update(*(c[i]), dt);
			w6-> Update(*(c[i]), dt);
			w7-> Update(*(c[i]), dt);
			w8-> Update(*(c[i]), dt);
			w9-> Update(*(c[i]), dt);
			w10->Update(*(c[i]), dt);
			w11->Update(*(c[i]), dt);
			w12->Update(*(c[i]), dt);
			w13->Update(*(c[i]), dt);
			w14->Update(*(c[i]), dt);
			w15->Update(*(c[i]), dt);
			w16->Update(*(c[i]), dt);
			w17->Update(*(c[i]), dt);
			w18->Update(*(c[i]), dt);
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

		coin1->Update(*player, dt);
		coin2->Update(*player, dt);
		coin3->Update(*player, dt);
		//archerTower->Update(*player, dt);
		up1->Update(*player, dt);
		up2->Update(*player, dt);
		next->Update(*player, dt);
		player->Update(*player, dt);
		ui->UI_Update(player);

		w1 ->Update(*player, dt);
		w2 ->Update(*player, dt);
		w3 ->Update(*player, dt);
		w4 ->Update(*player, dt);
		w5 ->Update(*player, dt);
		w6 ->Update(*player, dt);
		w7 ->Update(*player, dt);
		w8 ->Update(*player, dt);
		w9 ->Update(*player, dt);
		w10->Update(*player, dt);
		w11->Update(*player, dt);
		w12->Update(*player, dt);
		w13->Update(*player, dt);
		w14->Update(*player, dt);
		w15->Update(*player, dt);
		w16->Update(*player, dt);
		w17->Update(*player, dt);
		w18->Update(*player, dt);

		std::cout << (int)player->PosX <<", "<< (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 200, 120, player->GetFacing());

		BG->Render_Object(*M_BG);

		//archerTower->Render();
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
		for (Barrier& elem : barriers)
		{
			elem.Render();
		}
		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Render();
		}
		coin1->Render();
		coin2->Render();
		coin3->Render();


		//w1-> Render();
		//w2-> Render();
		//w3-> Render();
		//w4-> Render();
		//w5-> Render();
		//w6-> Render();
		//w7-> Render();
		//w8-> Render();
		//w9-> Render();
		//w10->Render();
		//w11->Render();
		//w12->Render();
		//w13->Render();
		//w14->Render();
		//w15->Render();
		//w16->Render();
		//w17->Render();
		//w18->Render();


		next->Render();

		player->Render();
		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		ui->Render();

	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete Audio;

		delete w1;
		delete w2;
		delete w3;
		delete w4;
		delete w5;
		delete w6;
		delete w7;
		delete w8;
		delete w9;
		delete w10;
		delete w11;
		delete w12;
		delete w13;
		delete w14;
		delete w15;
		delete w16;
		delete w17;
		delete w18;
		
		//delete archerTower;
		delete up1;
		delete up2;
		delete coin1;
		delete coin2;
		delete coin3;
		delete next;
		delete ui;
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