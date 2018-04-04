#include "Stage1_3.h"

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	Audio_Engine* Audio;
	UI* ui;
	AEVec2 startpos = { -450, -250 };

	int** MapData;
	int Map_Width;
	int Map_Height;

	//std::vector<Floor> floors;
	//std::vector<Wall> walls;
	std::vector<Block> blocks;
	LevelChangePlatform *next;
	std::vector<Characters*> c;

	//Sprite* COIN_SPRITE;//pickups
	//Sprite* HP_SPRITE;
	//Sprite* DMG_SPRITE;
	//Sprite* SPD_SPRITE;

	Sprite* WALL_SPRITE;
	Sprite* LCPLAT_SPRITE;
	Sprite* FLOOR_SPRITE;
}

namespace Stage1_3
{
	void Load(void)
	{
		// Reads in map data for this level
		if (!Import_MapData(".//Levels/level1-3.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		// Textures for static objects
		WALL_SPRITE   = new Sprite{ CreateFloor(1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		LCPLAT_SPRITE = new Sprite{ CreatePlatform(2.0f, 3.0f, ".//Textures/Win_Platform.png") };
		FLOOR_SPRITE  = new Sprite{ CreateFloor(1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };

		// Texture and transform matrix for BG
		BG = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		// Player creation
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));

		// Audio and UI
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Lancelot_BGM.mp3"); } };
		ui = new UI{ player };

		// Placement for level change platform
		next = new LevelChangePlatform{LCPLAT_SPRITE, 500.0f,  -300.0f };
	}

	void Init(void)
	{
		Audio->Play(0);
		Audio->SetLoop(0, 1);

		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_FLOOR)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					blocks.push_back(Block{ FLOOR_SPRITE, Convert_X(f_x) , Convert_Y(f_y) });
				}
				/*if (MapData[y][x] == OBJ_WALL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					walls.push_back(Wall{ WALL_SPRITE, Convert_X(f_x) , Convert_Y(f_y) });
				}*/
			}
		}
		c.push_back(Create_Boss_AI(LANCELOT));
		c[0]->SetActive(true);
		player->SetActive(true);
	}

	void Update(float dt)
	{
		Audio->Update();
		
		c[0]->Update(*player, dt);
		if (c[0]->Get_HP() <= 0)
		{
			next->Update(*player, dt);
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

		player->Update(*player, dt);
		ui->UI_Update(player);

		//std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		CamFollow(player->Transform_, 200, 120, player->GetFacing());
		CamStatic();
		BG->Render_Object(*M_BG);

		for (Block& elem : blocks)
		{
			elem.Render();
		}

		c[0]->Render();
		if (c[0]->Get_HP() <= 0)
		{
			next->Render();
		}
		player->Render();
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		ui->Render();
	}

	void Free(void)
	{
		//floors.clear();
		//walls.clear();
		blocks.clear();
		
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
		delete WALL_SPRITE;
		delete LCPLAT_SPRITE;
		delete FLOOR_SPRITE;
		//delete COIN_SPRITE;//pickups
		//delete HP_SPRITE;
		//delete DMG_SPRITE;
		//delete SPD_SPRITE;
		
		delete player;
		delete BG;
		delete M_BG;
		delete Audio;
		delete ui;
		delete next;
	}
}