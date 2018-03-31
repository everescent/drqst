#include "Stage1_2.h"

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
	PickUp *coin1, *coin2, *coin3, *hp;
	std::vector<Characters*> c;

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

namespace Stage1_2
{
	void Load(void)
	{		
		COIN_SPRITE    = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/coin.png", 1.0f) };
		HP_SPRITE      = new Sprite{ S_CreateSquare   (50.0f,  ".//Textures/hp.png", 1.0f) };
		DMG_SPRITE     = new Sprite{ S_CreateSquare   (50.0f,  ".//Textures/Fireball.png", 1.0f) };
		SPD_SPRITE     = new Sprite{ S_CreateSquare   (50.0f,  ".//Textures/spd.png", 1.0f) };
		BARRIER_SPRITE = new Sprite{ S_CreateSquare   (130.0f, ".//Textures/box.png") };
		WALL_SPRITE    = new Sprite{ CreateFloor      (1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		PLAT_SPRITE    = new Sprite{ CreatePlatform   (1.0f, 1.0f, ".//Textures/Cobblestone.png") };
		LCPLAT_SPRITE  = new Sprite{ CreatePlatform   (2.0f, 3.0f, ".//Textures/Win_Platform.png") };
		FLOOR_SPRITE   = new Sprite{ CreateFloor      (1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		TOWER_SPRITE   = new Sprite{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png") };
		SIGN_SPRITE    = new Sprite{ S_CreateSquare   (70.0f, ".//Textures/sign.png") };

		BG = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		AEVec2 startpos = { -310, -615 };
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));

		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_1_BGM.mp3"); } };
		ui = new UI(player);
		if (!Import_MapData(".//Levels/level1-2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		next = new LevelChangePlatform {LCPLAT_SPRITE, 7500.0f,  240.0f };

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

		c.push_back(Create_Basic_AI(GRUNT , AEVec2{  727.0f ,  105.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 1895.0f , -165.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 2335.0f ,  195.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 3200.0f , -600.0f }));
		c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 3500.0f , -600.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 3800.0f , -600.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 5200.0f ,  210.0f }));
		//c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 5800.0f ,  195.0f }));
		c.push_back(Create_Basic_AI(GRUNT , AEVec2{ 6450.0f ,  195.0f }));
		//c.push_back(Create_Basic_AI(ARCHER, AEVec2{ 6600.0f ,  300.0f }));
		c.push_back(Create_Basic_AI(KNIGHT, AEVec2{ 7200.0f ,  300.0f }));
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
					floors.push_back(Floor{ FLOOR_SPRITE, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_WALL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					walls.push_back(Wall{ WALL_SPRITE, Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_GRUNT)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(GRUNT, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) }));
				}
				if (MapData[y][x] == OBJ_ARCHER)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(ARCHER, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) }));
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
		next->Update(*player, dt);
		player->Update(*player, dt);
		CamFollow(player->Transform_, 200, 120, player->GetFacing());
		ui->UI_Update(player);

		std::cout << (int)player->PosX <<", "<< (int)player->PosY << std::endl;
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
		for (Wall& elem : walls)
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
		hp->Render();
		next->Render();

		player->Render();
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		ui->Render();

	}

	void Free(void)
	{
		delete player;
		delete BG;
		delete M_BG;
		delete Audio;

		delete ui;

		for (int y = 0; y < Map_Height; ++y)
		{
			delete[] MapData[y];
		}
		delete[] MapData;

		platforms.clear();
		floors.clear();
		walls.clear();
		barriers.clear();

		delete coin1;
		delete coin2;
		delete coin3;
		delete hp;
		delete next;

		for (size_t i = 0; i < c.size(); ++i)
		{
			delete c[i];
		}
		c.clear();
		
		// 11 sprites
		delete COIN_SPRITE;//pickups
		delete HP_SPRITE;
		delete DMG_SPRITE;
		delete SPD_SPRITE;
		delete BARRIER_SPRITE;//objs
		delete WALL_SPRITE;
		delete PLAT_SPRITE;
		delete LCPLAT_SPRITE;
		delete FLOOR_SPRITE;
		delete TOWER_SPRITE;
		delete SIGN_SPRITE;
	}

	void Unload(void)
	{

	}
}