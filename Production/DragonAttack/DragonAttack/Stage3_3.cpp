#include "Stage3_3.h"

namespace
{
	Dragon *player;
	Sprite *BG;
	Transform *M_BG;
	Audio_Engine* audio;
	UI* ui;

	int** MapData;
	int Map_Width;
	int Map_Height;

	//std::vector<Floor> floors;
	//std::vector<Wall> walls;
	std::vector<Platform> platforms;
	std::vector<Block> blocks;
	King_Arthur* last_boss;

	//Sprite* HP_SPRITE;
	//Sprite* DMG_SPRITE;
	//Sprite* SPD_SPRITE;
	Sprite* wall_sprite;
	Sprite* floor_sprite;
	Sprite* plat_sprite;

    BOSS_PHASE curr_phase;
}

namespace Stage3_3
{
	void Load(void)
	{
		if (!Import_MapData(".//Levels/level3-3.txt", MapData, Map_Width, Map_Height)) { GSM::next = GS_QUIT; }
		wall_sprite  = new Sprite{ S_CreateRectangle(50.0f, 50.0f, ".//Textures/download.jpg") };
		floor_sprite = new Sprite{ CreateFloor(1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		plat_sprite  = new Sprite{ CreatePlatform(1.0f, 1.0f, ".//Textures/Cobblestone.png") };

		BG   = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		AEVec2 startpos = { -450, -250 };
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));


        last_boss = dynamic_cast<King_Arthur*>(Create_Boss_AI(KING_ARTHUR));
        player->PosY = last_boss->PosY;
        player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/KingArthur_BGM.mp3"); } };
		ui    = new UI{ player };
	}

	void Init(void)
	{
		audio->Play(0);
		audio->SetLoop(0, 1);

        BG->SetRGB(0.5f, 0.5f, 0.5f);
		
		player->SetActive(true);

		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{	
				if (MapData[y][x] == OBJ_PLATFORM)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					platforms.push_back(Platform{ plat_sprite,Convert_X(f_x) , Convert_Y(f_y) });
				}
				if (MapData[y][x] == OBJ_FLOOR)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					blocks.push_back(Block{ floor_sprite, Convert_X(f_x) , Convert_Y(f_y) });
				}
				/*if (MapData[y][x] == OBJ_WALL)
				{
					float f_x = (float)x;
					float f_y = (float)y;
					walls.push_back(Wall{ wall_sprite, Convert_X(f_x) , Convert_Y(f_y) });
				}*/
			}
		}
	}

	void Update(float dt)
	{
	    audio->Update();

       curr_phase = last_boss->Get_Phase();

        last_boss->Update(*player, dt);

        for (Block& elem : blocks)
        {
            elem.Update(*player, dt);

            if (curr_phase == PHASE_2)
            {
                auto& mobs = last_boss->Get_Mobs();

                for (auto& elem1 : mobs)
                {
                    elem.Update(*elem1, dt);
                }
                
            }
        }

        /*for (Wall& elem : walls)
        {
            elem.Update(*player, dt);
        }*/

        for (Platform& elem : platforms)
        {
            switch (curr_phase)
            {
            case PHASE_1: case PHASE_3:

                if (elem.Collision_.Get_MinPoint().y == -200)
                    elem.Update(*player, dt);
                break;
            case PHASE_2:
                if(elem.Collision_.Get_MinPoint().y != -200)
                    elem.Update(*player, dt);
                break;
            default: break;
            }
        }

        player->Update(*player, dt);
        ui->UI_Update(player);

	}

	void Draw(void)
	{
        CamFollow(player->Transform_, 200, 120, player->GetFacing());
        CamStatic();
        BG->Render_Object(*M_BG);

        /*for (Floor& elem : floors)
        {
            elem.Render();
        }*/

		for (Block& elem : blocks)
		{
			elem.Render();
		}
        for (Platform& elem : platforms)
        {
            switch (curr_phase)
            {
            case PHASE_1: case PHASE_3:

                if (elem.Collision_.Get_MinPoint().y == -200)
                    elem.Render();
                break;
            case PHASE_2:
                if (elem.Collision_.Get_MinPoint().y != -200)
                    elem.Render();
                break;
            default: break;
            }
        }
		
		last_boss->Render();
        
        player->Render();
        
        ui->Render();
	}

	void Free(void)
	{
		delete player;
		delete BG;
		delete M_BG;
		delete audio;
		delete ui;

		for (int y = 0; y < Map_Height; ++y)
		{
			delete[] MapData[y];
		}
		delete[] MapData;

		//floors.clear();
		//walls.clear();
		blocks.clear();
		platforms.clear();

		delete last_boss;
		delete wall_sprite;
		delete floor_sprite;
		delete plat_sprite;
	}

	void Unload(void)
	{
		/*delete wall_sprite;
		delete floor_sprite;
		delete plat_sprite;
		delete last_boss;
		delete BG;
		delete M_BG;
		delete audio;
		delete ui;

        for (int y = 0; y < Map_Height; ++y)
        {
            delete[] MapData[y];
        }
        delete[] MapData;*/
	}
}
