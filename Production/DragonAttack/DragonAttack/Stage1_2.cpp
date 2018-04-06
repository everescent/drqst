#include "Stage1_2.h"

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	Audio_Engine* Audio;
	UI* ui;
	AEVec2 startpos = { -310, -615 };

	int** MapData;
	int Map_Width;
	int Map_Height;

	std::vector<Platform>    platforms;
	std::vector<Block>       blocks;
	std::vector<Barrier>     barriers;
	std::vector<PickUp>      PU;
	std::vector<Characters*> c;

	LevelChangePlatform *next;

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

	Pause* pause;
	bool pause_bool = false;
}

namespace Stage1_2
{
	void Load(void)
	{
		// Reads in map data for this level
		if (!Import_MapData(".//Levels/level1-2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		// Textures for pick ups
		COIN_SPRITE    = new Sprite{ S_CreateSquare   (35.0f, ".//Textures/coin.png", 1.0f) };
		HP_SPRITE      = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/hp.png", 1.0f) };
		DMG_SPRITE     = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/Fireball.png", 1.0f) };
		SPD_SPRITE     = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/spd.png", 1.0f) };
		INVUL_SPRITE   = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/invul.png", 1.0f) };

		// Textures for static objects
		BARRIER_SPRITE = new Sprite{ S_CreateSquare   (130.0f, ".//Textures/box.png") };
		SIGN_SPRITE    = new Sprite{ S_CreateSquare   (70.0f, ".//Textures/sign.png") };
		WALL_SPRITE    = new Sprite{ CreateFloor      (1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		FLOOR_SPRITE   = new Sprite{ CreateFloor      (1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		PLAT_SPRITE    = new Sprite{ CreatePlatform   (1.0f, 1.0f, ".//Textures/Cobblestone.png") };
		LCPLAT_SPRITE  = new Sprite{ CreatePlatform   (2.0f, 3.0f, ".//Textures/Win_Platform.png") };
		TOWER_SPRITE   = new Sprite{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png") };

		// Texture and transform matrix for BG
		BG   = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		// Player creation
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));

		// Audio and UI
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_1_BGM.mp3"); } };
		ui    = new UI{ player };

		// Placement for level change platform
		next = new LevelChangePlatform {LCPLAT_SPRITE, 7300.0f,  240.0f };

		pause = new Pause{}; 
	}

	void Init(void)
	{
		Audio->Play(0); // Plays selected track
		Audio->SetLoop(0, 1); // Loops the selected track

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
					blocks.push_back(Block{ FLOOR_SPRITE, Convert_X(f_x) , Convert_Y(f_y) });
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
				if (MapData[y][x] == OBJ_MAGE)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(MAGE, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) }));
				}
				if (MapData[y][x] == OBJ_KNIGHT)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(KNIGHT, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) }));
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
		pause->Update(pause_bool);
		if (!pause_bool) 
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
				// added collision for AI
				for (size_t i = 0; i < c.size(); ++i)
				{
					elem.Update(*(c[i]), dt);
				}
				elem.Update(*player, dt);
			}
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
			ui->UI_Update(player, dt);
		}

		//std::cout << (int)player->PosX <<", "<< (int)player->PosY << std::endl;

		
	}

	void Draw(void)
	{
		BG->Render_Object(*M_BG);

		for (Platform& elem : platforms)
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
		for (Barrier& elem : barriers)
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

		if (pause_bool) pause->Render();
	}

	void Free(void)
	{
		platforms.clear();
		blocks.clear();
		PU.clear();
		barriers.clear();

		for (size_t i = 0; i < c.size(); ++i)
		{
			delete c[i];
		}
		c.clear();
	}

	void Unload(void)
	{
		// Delete map data
		for (int y = 0; y < Map_Height; ++y)
		{
			delete[] MapData[y];
		}
		delete[] MapData;

		// Delete Sprites
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

		delete player;
		delete BG;
		delete M_BG;
		delete Audio;
		delete ui;
		delete next;

		delete pause;
	}
}