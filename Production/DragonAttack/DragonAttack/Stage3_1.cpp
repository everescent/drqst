/* Start Header ************************************************************************/
/*!
\file	 Stage3_1.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
Implementation for the stage 3-1 game state.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Stage3_1.h"

namespace
{
	Dragon        *player;						   // Player texture
	Sprite        *BG;							   // Background texture
	Transform     *M_BG, *M_BG2;				   // Transformation matrix for background texture
	UI            *ui;							   // User interface
	Audio_Engine  *Audio;						   // Audio for current stage
	Pause         *pause;						   // Pause screen

	bool pause_bool = false;					   // For checking if the game is currently paused
	const AEVec2        startpos = { -320, -700 }; // The start position for the player
	float Camdown = 120.0f;						   // For moving the camera downward

	int ** MapData;								   // Stores the binary map data for this stage
	int    Map_Width;							   // Width of the map
	int    Map_Height;							   // Height of the map

	std::vector<Platform>     platforms;		   // Vector for holding platforms
	std::vector<Barrier>      barriers;			   // Vector for holding barriers 
	std::vector<Block>        blocks;			   // Vector for holding blocks 
	std::vector<PickUp>       PU;				   // Vector for holding pickups 
	std::vector<Characters*>  c;				   // Vector for holding AI 
												   
	LevelChangePlatform *next;					   // Level change platform
	
	Sprite black;								   // Fade effect sprite
	Transform b_m;								   // Transformation matrix for the fading in
	Transform b_m2;								   // Transformation matrix for the fading out

	float timer = 3.0f;							   // Default timer for the fade effect
	bool FadeIn = true;							   // Fade in check
	bool FadeOut = false;						   // Fade out check
	f32 camX, camY;								   // Camera positions
	static float vis = 1.0f;					   // Fade effect visibility

	Sprite*  COIN_SPRITE;						   // Coin pickup sprite	 					
	Sprite*  HP_SPRITE;							   // Health pickup sprite	 						
	Sprite*  DMG_SPRITE;						   // Damage pickup sprite							
	Sprite*  SPD_SPRITE;						   // Speed pickup sprite	
	Sprite*  INVUL_SPRITE;						   // Invulnerability pickup sprite

	Sprite*  BARRIER_SPRITE;					   // Barrier sprite							
	Sprite*  WALL_SPRITE;						   // Wall	sprite	
	Sprite*  PLAT_SPRITE;						   // Platform	 sprite
	Sprite*  LCPLAT_SPRITE;						   // Level change platform sprite
	Sprite*  FLOOR_SPRITE;						   // Floor sprite
	Sprite*  TOWER_SPRITE;						   // Tower sprite
	Sprite*  SIGN_SPRITE;						   // Sign sprite
}												   

namespace Stage3_1
{
	/**************************************************************************************
	//
	// Loads the variables that are needed
	//
	**************************************************************************************/
	void Load(void)
	{
		// Reads in map data for this level
		if (!Import_MapData(".//Levels/level3-1.txt", MapData, Map_Width, Map_Height)) { AEGfxExit(); }

		// Textures for pick ups
		COIN_SPRITE    = new Sprite{ S_CreateSquare   (35.0f, ".//Textures/coin.png", 1.0f) };
		HP_SPRITE      = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/hp.png", 1.0f) };
		DMG_SPRITE     = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/Fireball.png", 1.0f) };
		SPD_SPRITE     = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/spd.png", 1.0f) };
		INVUL_SPRITE   = new Sprite{ S_CreateSquare   (50.0f, ".//Textures/invul.png", 1.0f) };

		// Textures for static objects
		BARRIER_SPRITE = new Sprite{ S_CreateSquare   (130.0f,         ".//Textures/box.png") };
		SIGN_SPRITE    = new Sprite{ S_CreateSquare   (70.0f,          ".//Textures/sign.png") };
		WALL_SPRITE    = new Sprite{ CreateFloor      (1.0f,           ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		FLOOR_SPRITE   = new Sprite{ CreateFloor      (1.0f,           ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		PLAT_SPRITE    = new Sprite{ CreatePlatform   (1.0f, 1.0f,     ".//Textures/Cobblestone.png") };
		LCPLAT_SPRITE  = new Sprite{ CreatePlatform   (2.0f, 3.0f,     ".//Textures/Win_Platform.png") };
		TOWER_SPRITE   = new Sprite{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png") };

		// Texture and transformation matrix for BG
		BG = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage2.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		M_BG2 = new Transform{};
		M_BG2->SetTranslate(0.0f, -1440.0f);
		M_BG2->Concat();

		// Audio
		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_3_BGM.mp3"); } };

		// Placement for level change platform
		next = new LevelChangePlatform{ LCPLAT_SPRITE, 7450.0f, -1100.0f };

		// Pause screen
		pause = new Pause{};

		// Fade in texture
		black = CreateBG(1.5f, 1.5f, ".//Textures/Black_BG.png");

		// Environment placement
		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_PLATFORM)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					platforms.push_back(Platform{ PLAT_SPRITE, Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector
				}
				if (MapData[y][x] == OBJ_FLOOR)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					blocks.push_back(Block{ FLOOR_SPRITE,Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector
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

		// Loops selected track
		Audio->SetLoop(0, FMOD_LOOP_NORMAL);

		// Pause the music and set volume to 0 if current state is muted
		if (Audio_Engine::MUTE_)
		{
			Audio->SetVolume(0, 0.0f); // Set volume to 0
			Audio->SetPause(0, true);  // Pause volume
		} 

		// Enemy and pickup placement
		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_GRUNT)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					c.push_back(Create_Basic_AI(GRUNT, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) })); // Push into vector
				}
				if (MapData[y][x] == OBJ_ARCHER)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					c.push_back(Create_Basic_AI(ARCHER, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) })); // Push into vector
				}
				if (MapData[y][x] == OBJ_KNIGHT)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					c.push_back(Create_Basic_AI(KNIGHT, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) })); // Push into vector
				}
				if (MapData[y][x] == OBJ_MAGE)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					c.push_back(Create_Basic_AI(MAGE, AEVec2{ Convert_X(f_x) ,  Convert_Y(f_y) })); // Push into vector
				}
				// Pickups
				if (MapData[y][x] == OBJ_COIN)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					PU.push_back(PickUp{ COIN_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						COIN, Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector with texture and collision box
				}
				if (MapData[y][x] == OBJ_HP)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					PU.push_back(PickUp{ HP_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						HP, Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector with texture and collision box
				}
				if (MapData[y][x] == OBJ_SPD)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					PU.push_back(PickUp{ SPD_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						SPD, Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector with texture and collision box
				}
				if (MapData[y][x] == OBJ_DMG)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					PU.push_back(PickUp{ DMG_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						DMG, Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector with texture and collision box
				}
				if (MapData[y][x] == OBJ_INVUL)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					PU.push_back(PickUp{ INVUL_SPRITE,
						Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
						INVUL, Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector with texture and collision box
				}
			}
		}

		// Set all AI as active
		for (size_t i = 0; i < c.size(); ++i)
			c[i]->SetActive(true);

		// Creation of player done in init so restarting the level will set the position
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));

		// Construct the ui on the player
		ui = new UI{ player };

		// Set player to active
		player->SetActive(true);

		// Reset player's Health and charge
		player->Set_HP(5);
		player->ResetCharge();

		// Set the first camera position instance
		CamFollow(player->Transform_, 200, 120, player->GetFacing(), true);
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
				// Mute all AI
				for (auto& elem : c)
					elem->Mute();

				player->Mute(); // Mute player

				// Mute the background music
				Audio->SetVolume(0, 0.0f);
				Audio->SetPause(0, true);
			}
			else
			{
				// Unmute all AI
				for (auto& elem : c)
					elem->Unmute();

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
					player->Update(*player, dt);
			}

			// Only updates AI if they are still active
			for (size_t i = 0; i < c.size(); ++i)
			{
				if (c[i]->IsActive())
				{
					c[i]->Update(*player, dt);
				}
			}

			// Updates all game objects for player and AI
			// Platforms
			for (Platform& elem : platforms)
			{
				// Added collision for AI
				for (size_t i = 0; i < c.size(); ++i)
				{
					elem.Update(*(c[i]), dt);
				}
				elem.Update(*player, dt);
			}

			// Blocks
			for (Block& elem : blocks)
			{
				for (size_t i = 0; i < c.size(); ++i)
				{
					elem.Update(*(c[i]), dt);
				}
				elem.Update(*player, dt);
			}

			// Barriers
			for (Barrier& elem : barriers)
			{
				elem.Update(*player, dt);
			}

			// Pickups
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
				Camdown += 4.0f; // Let the camera come up
			}
			CamFollow(player->Transform_, 200, Camdown, player->GetFacing()); // Update the camera
			next->Update(*player, dt, black, FadeOut); // Update the level change platform
			ui->UI_Update(player, dt); // Update the ui
		}
		else
		{
			Audio->SetPause(0, true); // Pausing BG music
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
		BG->Render_Object(*M_BG2);

		// Render all game objects
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

		// Clear pickups vector
		PU.clear();

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

		// Clear environment vectors
		platforms.clear();
		blocks.clear();
		barriers.clear();

		// Delete Sprites
		// Pickups
		delete COIN_SPRITE;
		delete HP_SPRITE;
		delete DMG_SPRITE;
		delete SPD_SPRITE;
		delete INVUL_SPRITE;

		// Objects
		delete BARRIER_SPRITE;
		delete WALL_SPRITE;
		delete PLAT_SPRITE;
		delete LCPLAT_SPRITE;
		delete FLOOR_SPRITE;
		delete TOWER_SPRITE;
		delete SIGN_SPRITE;

		// Background
		delete BG;
		delete M_BG;
		delete M_BG2;

		// Audio
		delete Audio;

		// Level change platform
		delete next;

		// Pause menu
		delete pause;
	}
}