/* Start Header ************************************************************************/
/*!
\file	 Stage1_3.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
Implementation for the stage 1-3 game state.

All content � 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Stage1_3.h"

namespace
{
	Dragon       *player;
	Sprite       *BG;
	Transform    *M_BG;
	UI           *ui;
	Audio_Engine *Audio;
	Pause        *pause;

	bool pause_bool = false;
	const AEVec2 startpos = { -450, -250 };

	int ** MapData;
	int    Map_Width;
	int    Map_Height;

	std::vector<Block>       blocks;
	std::vector<Characters*> c;

	LevelChangePlatform *next;

	Sprite black;
	Transform b_m;
	Transform b_m2;

	float timer = 3.0f;
	bool FadeIn = true;
	bool FadeOut = false;
	f32 camX, camY;
	static float vis = 1.0f;

	Sprite* WALL_SPRITE;
	Sprite* LCPLAT_SPRITE;
	Sprite* FLOOR_SPRITE;

	bool levelChange = false;
}

namespace Stage1_3
{
	void Load(void)
	{
		// Reads in map data for this level
		if (!Import_MapData(".//Levels/level1-3.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		// Textures for static objects
		WALL_SPRITE   = new Sprite{ CreateFloor   (1.0f,       ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		LCPLAT_SPRITE = new Sprite{ CreatePlatform(2.0f, 3.0f, ".//Textures/Win_Platform.png") };
		FLOOR_SPRITE  = new Sprite{ CreateFloor   (1.0f,       ".//Textures/Cobblestone.png", 1.0f, 1.0f) };

		// Texture and transform matrix for BG
		BG = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		// Audio and UI
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Lancelot_BGM.mp3"); } };

		// Placement for level change platform
		next = new LevelChangePlatform{LCPLAT_SPRITE, 500.0f,  -300.0f };

		pause = new Pause{};

		// Fade in texture
		black = CreateBG(1.5f, 1.5f, ".//Textures/Black_BG.png");

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
			}
		}

	}

	void Init(void)
	{
		Audio->Play(0);
		Audio->SetLoop(0, FMOD_LOOP_NORMAL);

		// pause the music and set volume to 0 if current state is muted
		if (Audio_Engine::MUTE_)
		{
			Audio->SetVolume(0, 0.0f); // set volume to 0
			Audio->SetPause(0, true);  // pause volume
		}

		c.push_back(Create_Boss_AI(LANCELOT));
		c[0]->SetActive(true);

		// Creation of player done in init so restarting the level will set the position
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));
		ui = new UI{ player };

		player->SetActive(true);

		// Reset player's Health and charge
		player->Set_HP(5);
		player->ResetCharge();
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

			// audio is mute
			if (Audio_Engine::MUTE_)
			{
				// mute lancelot
				c[0]->Mute();
				player->Mute();
				// mute the background music
				Audio->SetVolume(0, 0.0f);
				Audio->SetPause(0, true);
			}
			else
			{
				c[0]->Unmute();
				player->Unmute();
				// unmute the background music
				Audio->SetVolume(0, 1.0f);
				Audio->SetPause(0, false);
			}

			Audio->SetPause(0, false);
			Audio->Update();

			pause->Update(pause_bool,dt);

			if (!FadeIn)
			{
				if (player->GetUpdateFlag())
				{
					player->Update(*player, dt);
				}
					
				c[0]->Update(*player, dt);
			}

			if (c[0]->Get_HP() <= 0)
			{
				next->Update(*player, dt, black, FadeOut);
			}

			for (Block& elem : blocks)
			{
				for (size_t i = 0; i < c.size(); ++i)
				{
					elem.Update(*(c[i]), dt);
				}
				elem.Update(*player, dt);
			}

			ui->UI_Update(player, dt);
		}
		else 
		{
			Audio->SetPause(0, true);
			pause->Update(pause_bool,dt);
		}
		//std::cout << (int)player->PosX << ", " << (int)player->PosY << std::endl;
	}

	void Draw(void)
	{
		// removed camera functions for lancelot's CamShake()

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
		FadeIn = true;
		FadeOut = false;

		// Delete player and UI
		delete player;
		delete ui;

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

		blocks.clear();

		// Delete Sprites
		delete WALL_SPRITE;
		delete LCPLAT_SPRITE;
		delete FLOOR_SPRITE;
		
		delete BG;
		delete M_BG;
		delete Audio;
		delete next;
		delete pause;
	}
}