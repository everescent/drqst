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
	//std::vector<Floor> floors;
	//std::vector<Wall> walls;
	std::vector<Barrier> barriers;
	std::vector<Block> blocks;
	LevelChangePlatform *next;
	std::vector<Characters*> c;
	std::vector<PickUp> PU;

	Sprite* COIN_SPRITE;//pickups					 							   
	Sprite* HP_SPRITE;
	Sprite* DMG_SPRITE;
	Sprite* SPD_SPRITE;
	Sprite* INVUL_SPRITE;

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
		COIN_SPRITE = new Sprite{ S_CreateSquare(50.0f, ".//Textures/coin.png", 1.0f) };
		HP_SPRITE = new Sprite{ S_CreateSquare(50.0f,   ".//Textures/hp.png", 1.0f) };
		DMG_SPRITE = new Sprite{ S_CreateSquare(50.0f,  ".//Textures/Fireball.png", 1.0f) };
		SPD_SPRITE = new Sprite{ S_CreateSquare(50.0f,  ".//Textures/spd.png", 1.0f) };
		BARRIER_SPRITE = new Sprite{ S_CreateSquare(130.0f, ".//Textures/box.png") };
		WALL_SPRITE = new Sprite{ CreateFloor(1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		PLAT_SPRITE = new Sprite{ CreatePlatform(1.0f, 1.0f, ".//Textures/Cobblestone.png") };
		LCPLAT_SPRITE = new Sprite{ CreatePlatform(2.0f, 3.0f, ".//Textures/Win_Platform.png") };
		FLOOR_SPRITE = new Sprite{ CreateFloor(1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		TOWER_SPRITE = new Sprite{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png") };
		SIGN_SPRITE = new Sprite{ S_CreateSquare(70.0f, ".//Textures/sign.png") };
		INVUL_SPRITE = new Sprite{ S_CreateSquare(50.0f, ".//Textures/invul.png", 1.0f) };


		BG = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage2.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		AEVec2 startpos = {-440, -885};
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));

		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_2_BGM.mp3"); } };
		ui = new UI(player);
		if (!Import_MapData(".//Levels/level2-1.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }
		
		next = new LevelChangePlatform{ LCPLAT_SPRITE, 1180.0f,  -2685.0f };
	}


	void Init(void)
	{
		Audio->Play(0);
		Audio->SetLoop(0, 1);

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
					blocks.push_back(Block{ FLOOR_SPRITE,Convert_X(f_x) , Convert_Y(f_y) });
				}
				/*if (MapData[y][x] == OBJ_WALL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					walls.push_back(Wall{ WALL_SPRITE,Convert_X(f_x) , Convert_Y(f_y) });
				}*/
				if (MapData[y][x] == OBJ_GRUNT)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(GRUNT, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) }));
				}
				if (MapData[y][x] == OBJ_ARCHER)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(ARCHER, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) }));
				}
				if (MapData[y][x] == OBJ_MAGE)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(MAGE, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) }));
				}
				//pick ups
				if (MapData[y][x] == OBJ_COIN)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					PU.push_back(PickUp{ COIN_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						COIN, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_HP)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					PU.push_back(PickUp{ HP_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						HP, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_SPD)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					PU.push_back(PickUp{ SPD_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						SPD, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_DMG)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					PU.push_back(PickUp{ DMG_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						DMG, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_INVUL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					PU.push_back(PickUp{ INVUL_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						INVUL, Convert_X(f_x) , Convert_Y(f_y) });
				}
			}
		}

		for (size_t i = 0; i < c.size(); ++i)
			c[i]->SetActive(true);

		player->SetActive(true);
	}

	void Update(float dt)
	{
		Audio->Update();
		
		for (size_t i = 0; i < c.size(); ++i)
		{
			if (c[i]->IsActive())
			{
				c[i]->Update(*player, dt);
			}
		}

		for (Platform& elem : platforms)
		{
			elem.Update(*player, dt);
		}
		/*for (Floor& elem : floors)
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
		}*/
		for (Block& elem : blocks)
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
		for (PickUp& elem : PU)
		{
			elem.Update(*player, dt);
		}

		next->Update(*player, dt);
		player->Update(*player, dt);
		CamFollow(player->Transform_, 200, 120, player->GetFacing());
		ui->UI_Update(player);

		std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		BG->Render_Object(*M_BG);

		for (Platform& elem : platforms)
		{
			elem.Render();
		}
		/*for (Floor& elem : floors)
		{
			elem.Render();
		}
		for (Wall& elem : walls)
		{
			elem.Render();
		}*/
		for (Barrier& elem : barriers)
		{
			elem.Render();
		}
		for (Block& elem : blocks)
		{
			elem.Render();
		}
		for (PickUp& elem : PU)
		{
			elem.Render();
		}
		for (size_t i = 0; i < c.size(); ++i)
		{
			c[i]->Render();
		}
		next->Render();
		
		player->Render();
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		ui->Render();

		// Particle Effects
		PickUp::coin_particles->Render();
	}

	void Free(void)
	{
		delete BG;
		delete M_BG;
		delete player;
		delete Audio;

		delete next;
		delete ui;

		delete COIN_SPRITE;//pickups
		delete HP_SPRITE;
		delete DMG_SPRITE;
		delete SPD_SPRITE;
		delete INVUL_SPRITE;
		delete BARRIER_SPRITE;//objs
		delete WALL_SPRITE;
		delete PLAT_SPRITE;
		delete LCPLAT_SPRITE;
		delete FLOOR_SPRITE;
		delete TOWER_SPRITE;
		delete SIGN_SPRITE;

		platforms.clear();
		//floors.clear();
		//walls.clear();
		blocks.clear();
		barriers.clear();
		PU.clear();

		for (size_t i = 0; i < c.size(); ++i)
		{
			delete c[i];
		}
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