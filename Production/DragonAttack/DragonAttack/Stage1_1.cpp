#include "Stage1_1.h"

namespace
{
	Dragon       *player;
	Sprite       *BG;
	Transform    *M_BG;
	UI           *ui;
	Audio_Engine *Audio;
	Pause        *pause;

	bool pause_bool = false; // <------
	const AEVec2 startpos = { -320, -255 }; // <------

	int ** MapData;
	int    Map_Width;
	int    Map_Height;

	std::vector<Platform>    platforms;
	std::vector<Block>       blocks;
	std::vector<Barrier>     barriers;
	std::vector<PickUp>      PU;
	std::vector<Characters*> c;

	Tower                *archerTower;
	PickUp               *coin1;
	Barrier              *box1;
	Sign                 *s1, *s2, *s3, *s4, *s5, *s6;
	GameObject           *tut1, *tut2, *tut3, *tut4, *tut5, *tut6;
	LevelChangePlatform  *next;


	Sprite *TUT1_SPRITE;
	Sprite *TUT2_SPRITE;
	Sprite *TUT3_SPRITE;
	Sprite *TUT4_SPRITE;
	Sprite *TUT5_SPRITE;
	Sprite *TUT6_SPRITE;
		   
	Sprite *COIN_SPRITE;//pickups
	Sprite *HP_SPRITE;
	Sprite *DMG_SPRITE;
	Sprite *SPD_SPRITE;
	Sprite *INVUL_SPRITE;
		   
	Sprite *BARRIER_SPRITE;//objs
	Sprite *WALL_SPRITE;
	Sprite *PLAT_SPRITE;
	Sprite *LCPLAT_SPRITE;
	Sprite *FLOOR_SPRITE;
	Sprite *TOWER_SPRITE;
	Sprite *SIGN_SPRITE;
}

namespace Stage1_1
{
	void Load(void)
	{
		// Reads in map data for this level
		if (!Import_MapData(".//Levels/level1-1.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		// Textures for tutorial messages
		TUT1_SPRITE    = new Sprite { S_CreateRectangle(150.0f, 50.0f,  ".//Textures/Shoot_Tutorial_MSG.png") };
		TUT2_SPRITE    = new Sprite { S_CreateRectangle(230.0f, 80.0f,  ".//Textures/Boxes_Tutorial_MSG.png") };
		TUT3_SPRITE    = new Sprite { S_CreateRectangle(250.0f, 100.0f, ".//Textures/Enemy_Tutorial_MSG.png") };
		TUT4_SPRITE    = new Sprite { S_CreateRectangle(300.0f, 100.0f, ".//Textures/Platforming_Tutorial_MSG.png") };
		TUT5_SPRITE    = new Sprite { S_CreateRectangle(300.0f, 100.0f, ".//Textures/MegaFireball_Tutorial_MSG.png") };
		TUT6_SPRITE    = new Sprite { S_CreateRectangle(200.0f, 80.0f,  ".//Textures/PowerUp_Tutorial_MSG.png") };

		// Textures for pick ups
		COIN_SPRITE    = new Sprite { S_CreateSquare   (35.0f, ".//Textures/coin.png", 1.0f) };
		HP_SPRITE      = new Sprite { S_CreateSquare   (50.0f, ".//Textures/hp.png", 1.0f) };
		DMG_SPRITE     = new Sprite { S_CreateSquare   (50.0f, ".//Textures/Fireball.png", 1.0f) };
		SPD_SPRITE     = new Sprite { S_CreateSquare   (50.0f, ".//Textures/spd.png", 1.0f) };
		INVUL_SPRITE   = new Sprite { S_CreateSquare   (50.0f, ".//Textures/invul.png", 1.0f) };

		// Textures for static objects
		BARRIER_SPRITE = new Sprite { S_CreateSquare   (130.0f,         ".//Textures/box.png") };
		SIGN_SPRITE    = new Sprite { S_CreateSquare   (70.0f,          ".//Textures/sign.png") };
		WALL_SPRITE    = new Sprite { CreateFloor      (1.0f,           ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		FLOOR_SPRITE   = new Sprite { CreateFloor      (1.0f,           ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		PLAT_SPRITE    = new Sprite { CreatePlatform   (1.0f, 1.0f,     ".//Textures/Cobblestone.png") };
		LCPLAT_SPRITE  = new Sprite { CreatePlatform   (2.0f, 3.0f,     ".//Textures/Win_Platform.png") };
		TOWER_SPRITE   = new Sprite { S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png") };

		// Texture and transform matrix for BG
		BG     = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG   = new Transform{};

		// Audio and UI
		Audio  = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_1_BGM.mp3"); } };

		// Placement for specific objects
		archerTower = new Tower  { TOWER_SPRITE, 4800.0f, 0.0f };
		box1        = new Barrier{ BARRIER_SPRITE, 1500.0f, -235.0f };
		coin1       = new PickUp { COIN_SPRITE, Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
								   COIN, 1500.0f, -210.0f };
		next        = new LevelChangePlatform{ LCPLAT_SPRITE, 6550.0f, -50.0f };

		// Tutorial Messages/Signs
		s1 = new Sign{ SIGN_SPRITE, 400.0f, -255.0f };
		tut1 = new GameObject{ TUT1_SPRITE, Col_Comp() };
		s2 = new Sign{ SIGN_SPRITE, 1270.0f, -255.0f };
		tut2 = new GameObject{ TUT2_SPRITE, Col_Comp() };
		s3 = new Sign{ SIGN_SPRITE, 1650.0f, -255.0f };
		tut3 = new GameObject{ TUT3_SPRITE, Col_Comp() };
		s4 = new Sign{ SIGN_SPRITE, 2600.0f, -255.0f };
		tut4 = new GameObject{ TUT4_SPRITE, Col_Comp() };
		s5 = new Sign{ SIGN_SPRITE, 4200.0f, -75.0f };
		tut5 = new GameObject{ TUT5_SPRITE, Col_Comp() };
		s6 = new Sign{ SIGN_SPRITE, 5200.0f, -75.0f };
		tut6 = new GameObject{ TUT6_SPRITE, Col_Comp() };

		pause = new Pause{}; // <------
	}

	void Init(void)
	{
		tut1->SetActive(true);
		tut2->SetActive(true);
		tut3->SetActive(true);
		tut4->SetActive(true);
		tut5->SetActive(true);
		tut6->SetActive(true);

		//place all alpha stuff here
		tut1->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut2->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut3->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut4->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut5->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut6->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		
		// Plays selected track
		Audio->Play(0);

		// Loops selected track
		Audio->SetLoop(0, 1);
		
		// Onject placement
		for (int y = 0; y < Map_Height; ++y) // MOVE TO LOAD()???
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_PLATFORM)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					platforms.push_back(Platform{ PLAT_SPRITE,Convert_X(f_x) , Convert_Y(f_y) });
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
					c.push_back(Create_Basic_AI(GRUNT, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) }));
				}
				if (MapData[y][x] == OBJ_ARCHER)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					c.push_back(Create_Basic_AI(ARCHER, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) }));
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
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos)); // <------
		ui = new UI{ player }; // <------

		player->SetActive(true);

		// Reset player's Health and charge
		player->Set_HP(3); // <------
		player->ResetCharge(); // <------
	}

	void Update(float dt)
	{
		if (!pause_bool) // <------
		{ 
			Audio->Update();
			pause->Update(pause_bool);

			player->Update(*player, dt); // <------

			s1->Update(*player, dt);
			tut1->Transform_.SetTranslate(400.0f, -20.0f);
			tut1->Transform_.Concat();

			s2->Update(*player, dt);
			tut2->Transform_.SetTranslate(1270.0f, -20.0f);
			tut2->Transform_.Concat();

			s3->Update(*player, dt);
			tut3->Transform_.SetTranslate(1650.0f, -20.0f);
			tut3->Transform_.Concat();

			s4->Update(*player, dt);
			tut4->Transform_.SetTranslate(2600.0f, -20.0f);
			tut4->Transform_.Concat();

			s5->Update(*player, dt);
			tut5->Transform_.SetTranslate(4200.0f, 160.0f);
			tut5->Transform_.Concat();

			s6->Update(*player, dt);
			tut6->Transform_.SetTranslate(5200.0f, 160.0f);
			tut6->Transform_.Concat();

			for (size_t i = 0; i < c.size(); ++i)
			{
				if (c[i]->IsActive())
				{
					c[i]->Update(*player, dt);
				}
				archerTower->Update(*(c[i]), dt);
			}

			if (!(box1->IsActive()))
			{
				coin1->Update(*player, dt);
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
				elem.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			}
			for (PickUp& elem : PU)
			{
				elem.Update(*player, dt);
			}

			archerTower->Update(*player, dt);
			box1->Update(*player, dt);

			CamFollow(player->Transform_, 200, 120, player->GetFacing());
			next->Update(*player, dt);
			ui->UI_Update(player, dt);
		}
		else // <--
		{
			Audio->SetPause(0, 1);
			pause->Update(pause_bool);
		}
		//std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		// Background render
		BG->Render_Object(*M_BG);

		// Tutorial Message Pop Ups
		s1->Render();
		if (s1->ShowTutorial)
		{
			tut1->Render();
		}
		s2->Render();
		if (s2->ShowTutorial)
		{
			tut2->Render();
		}
		s3->Render();
		if (s3->ShowTutorial)
		{
			tut3->Render();
		}
		s4->Render();
		if (s4->ShowTutorial)
		{
			tut4->Render();
		}
		s5->Render();
		if (s5->ShowTutorial)
		{
			tut5->Render();
		}
		s6->Render();
		if (s6->ShowTutorial)
		{
			tut6->Render();
		}

		if (!(box1->IsActive()))
		{
			coin1->Render();
		}
		archerTower->Render();
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

		box1->Render();
		player->Render();
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		next->Render();
		ui->Render();

		// Particle Effects
		PickUp::coin_particles->Render();
		
		if (pause_bool) pause->Render();
	}

	void Free(void)
	{
		// Delete player and UI
		delete player;
		delete ui;

		// Clear object vectors
		platforms.clear();
		blocks.clear();
		barriers.clear();
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

		// Delete Sprites
		delete TUT1_SPRITE;
		delete TUT2_SPRITE;
		delete TUT3_SPRITE;
		delete TUT4_SPRITE;
		delete TUT5_SPRITE;
		delete TUT6_SPRITE;

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
		delete Audio;

		// Tutorial sprites
		delete s1;
		delete s2;
		delete s3;
		delete s4;
		delete s5;
		delete s6;
		delete tut1;
		delete tut2;
		delete tut3;
		delete tut4;
		delete tut5;
		delete tut6;

		delete box1;
		delete archerTower;
		delete coin1;
		delete next;

		delete pause;
	}
}
