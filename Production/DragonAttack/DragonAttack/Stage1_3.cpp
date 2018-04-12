/* Start Header ************************************************************************/
/*!
\file	 Stage1_3.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
Implementation for the stage 1-3 game state.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Stage1_3.h"

// Global variables
namespace
{
	Dragon       *player;					// Player texture
	Sprite       *BG;						// First background texture
	Transform    *M_BG;						// Transformation matrix for first background texture
	UI           *ui;						// User interface
	Audio_Engine *Audio;					// Audio for current stage
	Pause        *pause;					// Pause screen

	bool pause_bool = false;				// For checking if the game is currently paused
	const AEVec2 startpos = { -450, -250 };	// The start position for the player
											
	int ** MapData;							// Stores the binary map data for this stage
	int    Map_Width;						// Width of the map
	int    Map_Height;						// Height of the map
											
	std::vector<Block>       blocks;		// Vector for holding blocks 
	std::vector<Characters*> c;				// Vector for holding AI 

	LevelChangePlatform *next;				// Level change platform

	Sprite black;							// Fade effect sprite
	Transform b_m;							// Transformation matrix for the fading in
	Transform b_m2;							// Transformation matrix for the fading out

	float timer = 3.0f;						// Default timer for the fade effect
	bool FadeIn = true;						// Fade in check
	bool FadeOut = false;					// Fade out check
	f32 camX, camY;							// Camera positions
	static float vis = 1.0f;				// Fade effect visibility

	Sprite* WALL_SPRITE;					// Wall	sprite
	Sprite* LCPLAT_SPRITE;					// Level change platform sprite
	Sprite* FLOOR_SPRITE;					// Floor sprite
}

namespace Stage1_3
{
	/**************************************************************************************
	//
	// Loads the variables that are needed
	//
	**************************************************************************************/
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

		// Audio
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Lancelot_BGM.mp3"); } };

		// Placement for level change platform
		next = new LevelChangePlatform{LCPLAT_SPRITE, 500.0f,  -300.0f };

		// Pause screen
		pause = new Pause{};

		// Fade in texture
		black = CreateBG(1.5f, 1.5f, ".//Textures/Black_BG.png");

		// Environmental placement
		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_FLOOR)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					blocks.push_back(Block{ FLOOR_SPRITE, Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector
				}
			}
		}

	}
	/**************************************************************************************
	//
	// Init the variables that are needed
	//
	**************************************************************************************/
	void Init(void)
	{
		// Plays selected track
		Audio->Play(0);

		// Loops the selected track
		Audio->SetLoop(0, FMOD_LOOP_NORMAL);

		// Pause the music and set volume to 0 if current state is muted
		if (Audio_Engine::MUTE_)
		{
			Audio->SetVolume(0, 0.0f); // Set volume to 0
			Audio->SetPause(0, true);  // Pause volume
		}

		// Create Boss AI
		c.push_back(Create_Boss_AI(LANCELOT));
		
		// Set boss to active
		c[0]->SetActive(true);

		// Creation of player done in init so restarting the level will set the position
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));

		// Construct the ui on the player
		ui = new UI{ player };

		// Set player to active
		player->SetActive(true);

		// Reset player's Health and charge
		player->Set_HP(5);
		player->ResetCharge();
	}
	/**************************************************************************************
	//
	// Updates the audio, character and AI behavior
	//
	**************************************************************************************/
	void Update(float dt)
	{
		// Fade transformation matrix
		AEGfxGetCamPosition(&camX, &camY);
		b_m.SetTranslate(camX, camY);
		b_m.Concat();

		// Checks if pause is true
		if (!pause_bool) 
		{
			// Fade In effect
			if (FadeIn)
			{
				black.SetAlphaTransBM(1.0f, vis, AE_GFX_BM_BLEND);
				vis -= 0.005f;
				timer -= dt;

				if (timer <= 0) // Sets to false once timer is over
				{
					FadeIn = false;
				}
			}

			if (Audio_Engine::MUTE_)
			{
				c[0]->Mute(); // Mute boss
				player->Mute(); // Mute player

				// Mute the background music
				Audio->SetVolume(0, 0.0f);
				Audio->SetPause(0, true);
			}
			else
			{
				c[0]->Unmute(); // Unmute boss
				player->Unmute(); // Unmute player

				// Unmute the background music
				Audio->SetVolume(0, 1.0f);
				Audio->SetPause(0, false);
			}

			// Set audio to be paused
			Audio->SetPause(0, false);

			// Update audio
			Audio->Update();

			// Update pause
			pause->Update(pause_bool,dt);

			if (!FadeIn) // If fade in has not finished, do not allow player to move
			{
				if (player->GetUpdateFlag()) // Prevents player update if flag is false
				{
					player->Update(*player, dt);
				}
				if (c[0]->IsActive()) // Only update boss if active
				{
					c[0]->Update(*player, dt);
				}
			}

			// Checks if the boss is defeated
			if (c[0]->Get_HP() <= 0)
			{
				next->Update(*player, dt, black, FadeOut); // Update the level change platform
			}

			// Update blocks
			for (Block& elem : blocks)
			{
				for (size_t i = 0; i < c.size(); ++i)
				{
					elem.Update(*(c[i]), dt);
				}
				elem.Update(*player, dt);
			}

			ui->UI_Update(player, dt); // Update the ui
		}
		else 
		{
			Audio->SetPause(0, true); // Pausing the BG music
			pause->Update(pause_bool,dt); // Update the pause screen
		}
	}
	/**************************************************************************************
	//
	// Render the variables that are needed
	//
	**************************************************************************************/
	void Draw(void)
	{
		// Background render
		BG->Render_Object(*M_BG);

		// Render all game objects
		for (Block& elem : blocks)
		{
			elem.Render();
		}

		// Render boss
		c[0]->Render();
		if (c[0]->Get_HP() <= 0)
		{
			next->Render(); // Render level change platform if boss is defeated
		}

		player->Render();
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		ui->Render();

		// Render fading effects
		if (FadeIn)
			black.Render_Object(b_m);
		if (FadeOut)
		{
			b_m2.SetTranslate(camX, camY);
			b_m2.Concat();
			black.Render_Object(b_m2);
		}

		// Render pause menu when player pauses
		if (pause_bool) pause->Render();
	}
	/**************************************************************************************
	//
	// Free the variables that were used
	//
	**************************************************************************************/
	void Free(void)
	{
		// Reset fade effect variables
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
		// Clear enemy vector
		c.clear();
	}
	/**************************************************************************************
	//
	// Unloads the variables that were used
	//
	**************************************************************************************/
	void Unload(void)
	{
		// Delete map data
		for (int y = 0; y < Map_Height; ++y)
		{
			delete[] MapData[y];
		}
		delete[] MapData;

		// Clear environment vector
		blocks.clear();

		// Delete Sprites
		// Objects
		delete WALL_SPRITE;
		delete LCPLAT_SPRITE;
		delete FLOOR_SPRITE;
		
		// Background
		delete BG;
		delete M_BG;

		// Audio
		delete Audio;

		// Level change platform
		delete next;

		// Pause menu
		delete pause;
	}
}