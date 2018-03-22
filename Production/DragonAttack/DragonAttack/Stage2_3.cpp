#include "Stage2_3.h"

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

namespace Stage2_3
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
		if (!Import_MapData("level2-3.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }
		
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