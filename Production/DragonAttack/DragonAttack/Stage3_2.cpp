#include "Stage3_2.h"

namespace
{
	Dragon       *player;
	Sprite       *BG;
	Transform    *M_BG, *M_BG2, *M_BG3;
	UI           *ui;
	Audio_Engine *Audio;
	Pause        *pause;

	bool pause_bool = false;
	const AEVec2 startpos = { -345, -2350 };
	float Camdown = 120.0f;

	int ** MapData;
	int    Map_Width;
	int    Map_Height;

	std::vector<Platform>     platforms;
	std::vector<Barrier>      barriers;
	std::vector<Block>        blocks;
	std::vector<PickUp>       PU;
	std::vector<Characters*>  c;

	LevelChangePlatform *next;

	Sprite black;
	Transform b_m;
	Transform b_m2;

	float timer = 3.0f;
	bool FadeIn = true;
	bool FadeOut = false;
	f32 camX, camY;
	static float vis = 1.0f;

	Sprite*  COIN_SPRITE;//pickups					 							   
	Sprite*  HP_SPRITE;
	Sprite*  DMG_SPRITE;
	Sprite*  SPD_SPRITE;
	Sprite*  INVUL_SPRITE;
		     
	Sprite*  BARRIER_SPRITE;//objs												   
	Sprite*  WALL_SPRITE;
	Sprite*  PLAT_SPRITE;
	Sprite*  LCPLAT_SPRITE;
	Sprite*  FLOOR_SPRITE;
	Sprite*  TOWER_SPRITE;
	Sprite*  SIGN_SPRITE;
}

namespace Stage3_2
{
	void Load(void)
	{
		// Reads in map data for this level
		if (!Import_MapData(".//Levels/level3-2.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		// Textures for pick ups
		COIN_SPRITE    = new Sprite{ S_CreateSquare   (35.0f, ".//Textures/coin.png", 1.0f) };
		HP_SPRITE      = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/hp.png", 1.0f) };
		DMG_SPRITE     = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/Fireball.png", 1.0f) };
		SPD_SPRITE     = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/spd.png", 1.0f) };
		INVUL_SPRITE   = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/invul.png", 1.0f) };
												      
		// Textures for static objects			      
		BARRIER_SPRITE = new Sprite{ S_CreateSquare   (130.0f,         ".//Textures/box.png") };
		SIGN_SPRITE    = new Sprite{ S_CreateSquare   (70.0f,          ".//Textures/sign.png") };
		PLAT_SPRITE    = new Sprite{ CreatePlatform   (1.0f, 1.0f,     ".//Textures/Cobblestone.png") };
		LCPLAT_SPRITE  = new Sprite{ CreatePlatform   (2.0f, 3.0f,     ".//Textures/Win_Platform.png") };
		WALL_SPRITE    = new Sprite{ CreateFloor      (1.0f,           ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		FLOOR_SPRITE   = new Sprite{ CreateFloor      (1.0f,           ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		TOWER_SPRITE   = new Sprite{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png") };

		// Texture and transformation matrix for BG
		BG = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage2.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		M_BG2 = new Transform{};
		M_BG2->SetTranslate(0.0f, -1440.0f);
		M_BG2->Concat();

		M_BG3 = new Transform{};
		M_BG3->SetTranslate(0.0f, -2880.0f);
		M_BG3->Concat();

		// Audio and UI
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_3_BGM.mp3"); } };

		// Placement for level change platform
		next = new LevelChangePlatform{ LCPLAT_SPRITE, 5000.0f,  220.0f };

		// Pause menu object
		pause = new Pause{};

		// Fade in texture
		black = CreateBG(1.5f, 1.5f, ".//Textures/Black_BG.png");

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
				if (MapData[y][x] == OBJ_KNIGHT)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(KNIGHT, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) }));
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

	}

	void Init(void)
	{
		// Plays selected track
		Audio->Play(0);

		// Loops selected track
		Audio->SetLoop(0, FMOD_LOOP_NORMAL);

		// Object placement
		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
		//		if (MapData[y][x] == OBJ_PLATFORM)
		//		{
		//			float f_x = (float)x;
		//			float f_y = (float)y;
		//			platforms.push_back(Platform{ PLAT_SPRITE, Convert_X(f_x) , Convert_Y(f_y) });
		//		}
		//		if (MapData[y][x] == OBJ_FLOOR)
		//		{
		//			float f_x = (float)x;
		//			float f_y = (float)y;
		//			blocks.push_back(Block{ FLOOR_SPRITE,Convert_X(f_x) , Convert_Y(f_y) });
		//		}
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
				if (MapData[y][x] == OBJ_KNIGHT)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(KNIGHT, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) }));
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

		// Creation of player done in init so restarting the level will set the position
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));
		ui = new UI{ player };

		player->SetActive(true);

		// Reset player's Health and charge
		player->Set_HP(5);
		player->ResetCharge();

		CamFollow(player->Transform_, 200, 120, player->GetFacing());
	}

	void Update(float dt)
	{
		// Fade transformation matrix
		AEGfxGetCamPosition(&camX, &camY);
		b_m.SetTranslate(camX, camY);
		b_m.Concat();

		
		if (!pause_bool)
		{
			// Fade In effect
			if (FadeIn)
			{
				//static float vis = 1.0f;
				black.SetAlphaTransBM(1.0f, vis, AE_GFX_BM_BLEND);
				vis -= 0.005f;

				timer -= dt;

				if (timer <= 0)
				{
					FadeIn = false;
				}
			}


			Audio->SetPause(0, false);
			Audio->Update();
			pause->Update(pause_bool, dt);

			if (!FadeIn)
			{
				player->Update(*player, dt);
			}

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

			// Camera down logic
			if (AEInputCheckCurr(AEVK_S))
			{
				if (Camdown > -250) // Setting lowest camera point
					Camdown -= 4.0f;
			}
			if (!AEInputCheckCurr(AEVK_S) && Camdown < 120)
			{
				Camdown += 4.0f;
			}
			CamFollow(player->Transform_, 200, Camdown, player->GetFacing());
			next->Update(*player, dt, black, FadeOut);
			ui->UI_Update(player, dt);
		}
		else
		{
			Audio->SetPause(0, true);
			pause->Update(pause_bool, dt);
		}
		//std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
		
	}

	void Draw(void)
	{
		// Background render
		BG->Render_Object(*M_BG);
		BG->Render_Object(*M_BG2);
		BG->Render_Object(*M_BG3);

		for (Platform& elem : platforms)
		{
			elem.Render();
		}
		for (Block& elem : blocks)
		{
			elem.Render();
		}
		for (Barrier& elem : barriers)
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

		player->Render();
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		next->Render();
		ui->Render();

		// Particle Effects
		PickUp::coin_particles->Render();

		if (FadeIn)
			black.Render_Object(b_m);
		if (FadeOut)
		{
			b_m2.SetTranslate(camX, camY);
			b_m2.Concat();
			black.Render_Object(b_m2);
		}

		if (pause_bool) pause->Render();
	}

	void Free(void)
	{
		timer = 3.0f;
		vis = 1.0f;
		// Delete player and UI
		delete player;
		delete ui;

		// Clear object vectors
		/*platforms.clear();
		blocks.clear();
		barriers.clear();*/
		PU.clear();

		// Delete enemies
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

		platforms.clear();
		blocks.clear();
		barriers.clear();
		//PU.clear();

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

		delete BG;
		delete M_BG;
		delete M_BG2;
		delete M_BG3;
		delete Audio;
		delete next;
		delete pause;
	}
}