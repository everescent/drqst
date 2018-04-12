/* Start Header ************************************************************************/
/*!
\file	 Stage1_1.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
Implementation for the stage 1-1 game state.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Stage1_1.h"

// Global Variables
namespace
{
	Dragon       *player;					// Player texture
	Sprite       *BG;						// First background texture
	Sprite		 *BG2;						// Second background texture
	Transform    *M_BG;						// Transformation matrix for first background texture
	Transform    *M_BG2;					// Transformation matrix for second background texture
	UI           *ui;						// User interface
	Audio_Engine *Audio;					// Audio for current stage
	Pause        *pause;					// Pause screen

	bool pause_bool = false;				// For checking if the game is currently paused
	const AEVec2 startpos = { -320, -255 }; // The start position for the player

	int ** MapData;							// Stores the binary map data for this stage
	int    Map_Width;						// Width of the map
	int    Map_Height;						// Height of the map

	std::vector<Platform>    platforms;		// Vector for holding platforms 
	std::vector<Block>       blocks;		// Vector for holding blocks 
	std::vector<Barrier>     barriers;		// Vector for holding barriers 
	std::vector<PickUp>      PU;			// Vector for holding pickups 
	std::vector<Characters*> c;				// Vector for holding AI 

	Tower        *archerTower;				// Archer Tower object
	PickUp       *coin1;					// Coin object
	Barrier      *box1;						// Box object
				 
	Sign         *s1, 						// 1st Sign object
				 *s2, 						// 2nd Sign object
				 *s3, 						// 3rd Sign object
				 *s4, 						// 4th Sign object
				 *s5, 						// 5th Sign object
				 *s6;						// 6th Sign object
				 
	GameObject   *tut1, 					// 1st tutorial message
				 *tut2, 					// 2nd tutorial message
				 *tut3, 					// 3rd tutorial message
				 *tut4, 					// 4th tutorial message
				 *tut5, 					// 5th tutorial message
				 *tut6;						// 6th tutorial message

	LevelChangePlatform  *next;				// Level change platform

	Sprite		black;						// Fade effect sprite
	Transform	b_m;						// Transformation matrix for the fading in
	Transform	b_m2;						// Transformation matrix for the fading out

	float timer = 3.0f;						// Default timer for the fade effect
	bool FadeIn = true;						// Fade in check
	bool FadeOut = false;					// Fade out check
	f32 camX, camY;							// Camera positions
	static float vis = 1.0f;				// Fade effect visibility
											   
	Sprite *TUT1_SPRITE;					// 1st tutorial sprite
	Sprite *TUT2_SPRITE;					// 2nd tutorial sprite
	Sprite *TUT3_SPRITE;					// 3rd tutorial sprite
	Sprite *TUT4_SPRITE;					// 4th tutorial sprite
	Sprite *TUT5_SPRITE;					// 5th tutorial sprite
	Sprite *TUT6_SPRITE;					// 6th tutorial sprite
		   									   
	Sprite *COIN_SPRITE;					// Coin pickup sprite
	Sprite *HP_SPRITE;						// Health pickup sprite
	Sprite *DMG_SPRITE;						// Damage pickup sprite
	Sprite *SPD_SPRITE;						// Speed pickup sprite
	Sprite *INVUL_SPRITE;					// Invulnerability pickup sprite

	Sprite *BARRIER_SPRITE;					// Barrier sprite
	Sprite *WALL_SPRITE;					// Wall	sprite
	Sprite *PLAT_SPRITE;					// Platform	 sprite
	Sprite *LCPLAT_SPRITE;					// Level change platform sprite
	Sprite *FLOOR_SPRITE;					// Floor sprite
	Sprite *TOWER_SPRITE;					// Tower sprite
	Sprite *SIGN_SPRITE;					// Sign sprite
}

namespace Stage1_1
{
	/**************************************************************************************
	//
	// Loads the variables that are needed
	//
	**************************************************************************************/
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

		BG2    = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1_Ground.png", 1.0f, 15.0f) };
		M_BG2  = new Transform{};
		M_BG2->SetTranslate(0.0f, -1440.0f);
		M_BG2->Concat();

		// Audio
		Audio  = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/Stage_1_BGM.mp3"); } };

		// Placement for level change platform
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
					platforms.push_back(Platform{ PLAT_SPRITE,Convert_X(f_x) , Convert_Y(f_y) }); // Push into vector
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
		// Set all tutorial objects to be active
		tut1->SetActive(true);
		tut2->SetActive(true);
		tut3->SetActive(true);
		tut4->SetActive(true);
		tut5->SetActive(true);
		tut6->SetActive(true);

		// Set alpha for tutorial sprites
		tut1->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut2->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut3->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut4->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut5->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		tut6->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		// Placement for specific objects
		archerTower = new Tower{ TOWER_SPRITE, 4800.0f, 0.0f };
		archerTower->Transform_.SetTranslate(4800.0f, 0.0f);
		archerTower->Transform_.Concat();

		box1 = new Barrier{ BARRIER_SPRITE, 1500.0f, -235.0f };
		box1->Transform_.SetTranslate(1500.0f, -235.0f);
		box1->Transform_.Concat();

		coin1 = new PickUp{ COIN_SPRITE, Col_Comp{ 0.0f - 25.0f, 0.0f - 25.0f, 0.0f + 25.0f, 0.0f + 25.0f, Rect },
			COIN, 1500.0f, -210.0f };
		coin1->Transform_.SetTranslate(1500.0f, -210.0f);
		coin1->Transform_.Concat();
		
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
				// AI
				if (MapData[y][x] == OBJ_GRUNT)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					c.push_back(Create_Basic_AI(GRUNT, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) })); // Push into vector
				}
				if (MapData[y][x] == OBJ_ARCHER)
				{
					float f_x = (float)x; // Cast into float
					float f_y = (float)y; // Cast into float
					c.push_back(Create_Basic_AI(ARCHER, AEVec2{ Convert_X(f_x) , Convert_Y(f_y) })); // Push into vector
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
		
		// Set all AI to active
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

			// Set audio to be unpaused
			Audio->SetPause(0, false);

			// Update audio
			Audio->Update();

			// Update pause
			pause->Update(pause_bool, dt);

			if (!FadeIn) // If fade in has not finished, do not allow player to move
			{
				if (player->GetUpdateFlag()) // Prevents player update if flag is false
					player->Update(*player, dt);
			}

			// Set locations for tutorial objects
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

			// Only updates AI if they are still active
			for (size_t i = 0; i < c.size(); ++i)
			{
				if (c[i]->IsActive())
				{
					c[i]->Update(*player, dt);
				}
				archerTower->Update(*(c[i]), dt);
			}

			// Only update coin if the box is not active
			if (!(box1->IsActive()))
			{
				coin1->Update(*player, dt);
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
				elem.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			}

			// Pickups
			for (PickUp& elem : PU)
			{
				elem.Update(*player, dt);
			}

			archerTower->Update(*player, dt); // Update tower with player
			box1->Update(*player, dt); // Update box with player

			CamFollow(player->Transform_, 200, 120, player->GetFacing()); // Update the camera
			next->Update(*player, dt, black, FadeOut); // Update the level change platform
			ui->UI_Update(player, dt); // Update the ui
		}
		else
		{
			Audio->SetPause(0, true); // Pausing BG music
			pause->Update(pause_bool, dt); // Update the pause screen
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
		BG2->Render_Object(*M_BG2);

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

		// Render the coin when box is destroyed
		if (!(box1->IsActive()))
		{
			coin1->Render();
		}

		// Render all game objects
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

		// Delete unique objects
		delete box1;
		delete archerTower;
		delete coin1;

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
		// Tutorial messages
		delete TUT1_SPRITE;
		delete TUT2_SPRITE;
		delete TUT3_SPRITE;
		delete TUT4_SPRITE;
		delete TUT5_SPRITE;
		delete TUT6_SPRITE;

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
		delete BG2;
		delete M_BG2;

		// Audio
		delete Audio;

		// Tutorial and sign objects
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
		delete next;

		// Pause menu
		delete pause;
	}
}
