/* Start Header ************************************************************************/
/*!
\file       Stage3_3.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Final stage of the boss

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Stage3_3.h"

namespace // global variables used in this file
{
	Dragon *          player;                     // player character
	Sprite *          BG;                         // background texture
	Transform *       M_BG;                       // transformation matrix for background
	Audio_Engine*     audio;                      // audio for current stage
	UI*               ui;                         // to display player's health
	AEVec2            startpos = { -450, -250 };  // spawn point of player
                                         
	int** MapData;                       // stores the binary map of the current stage
	int Map_Width;                       // width of the map
	int Map_Height;                      // height of the map

	std::vector<Platform> platforms;     // platforms for the stage
	std::vector<Block> blocks;           //
	King_Arthur* last_boss;              // the final boss ai

	//Sprite* HP_SPRITE;
	//Sprite* DMG_SPRITE;
	//Sprite* SPD_SPRITE;
	Sprite* wall_sprite;                 // texture for the wall  
	Sprite* floor_sprite;                // texture for the floor
	Sprite* plat_sprite;                 // texture for the platform

    BOSS_PHASE curr_phase;               // current phase of the boss
}

namespace Stage3_3
{
	/**************************************************************************************
	//
	// Loads the variables that are needed
	//
	**************************************************************************************/
	void Load(void)
	{
		// reads the map data from text file. If not found, exit the game
		if (!Import_MapData(".//Levels/level3-3.txt", MapData, Map_Width, Map_Height)) { GSM::next = GS_QUIT; }
		
		// textures for floor, wall and platform
		wall_sprite  = new Sprite{ S_CreateRectangle(50.0f, 50.0f, ".//Textures/download.jpg") };
		floor_sprite = new Sprite{ CreateFloor(1.0f, ".//Textures/Cobblestone.png", 1.0f, 1.0f) };
		plat_sprite  = new Sprite{ CreatePlatform(1.0f, 1.0f, ".//Textures/Cobblestone.png") };

		// texture and transformation matrix for background
		BG   = new Sprite{ CreateBG(22.0f, 2.0f, ".//Textures/BG_Stage1.png", 1.0f, 15.0f) };
		M_BG = new Transform{};

		// creating the player obj
		player = dynamic_cast<Dragon*>(Create_Basic_AI(DRAGON, startpos));
		player->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

        // audio and us used for the stage
		audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/KingArthur_BGM.mp3"); } };
		ui    = new UI{ player };

		// get the map data and create corresponding objects for the stage
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
		// play the audio for the stage
		audio->Play(0);
		audio->SetLoop(0, 1);

        BG->SetRGB(0.5f, 0.5f, 0.5f);
		
		// update player variables
		player->SetPos(startpos.x, startpos.y); // set the spawn location of dragon
		player->SetActive(true);                // set player to alive
		player->Set_HP(3);

		// creating the last boss obj
		last_boss = dynamic_cast<King_Arthur*>(Create_Boss_AI(KING_ARTHUR)); // cast obj from character to king arthur

		
	}

	/**************************************************************************************
	//
	// Updates the audio, character and boss behavior
	//
	**************************************************************************************/
	void Update(float dt)
	{
		// if king arthur has died, switch state to credit screen
		if (last_boss->Get_HP() <= 0)
		{
			SM::Set_Next(SS_QUIT);
			GSM::next = GS_CREDITS;
	    }
		else if (player->Get_HP() <= 0)
		{
			SM::Set_Next(SS_RESTART);  // change state to restart
			last_boss->Set_HP(0);      // kills the boss
		}

		//player->Set_Vulnerable(true);
	   // update the audio
	   audio->Update();

	   // get the current phase of the boss Ai
       curr_phase = last_boss->Get_Phase();

		// update the boss behavior
        last_boss->Update(*player, dt);

		// update the collision between AI/player and floor 
        for (Block& elem : blocks)
        {
            elem.Update(*player, dt);
			
			// only updates the ai if its in phase 2
            if (curr_phase == PHASE_2)
            {
                auto& mobs = last_boss->Get_Mobs();

                for (auto& elem1 : mobs)
                {
                    elem.Update(*elem1, dt);
                }
                
            }
        }

        // update collision between player and platforms
		// different phases have different platforms for the fight
		for (Platform& elem : platforms)
        {
            // check collision with corresponding platforms depending on current phase
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

        // update the player behavior and UI
		player->Update(*player, dt);
        ui->UI_Update(player);
	}
	/**************************************************************************************
	//
	// Render the variables that are needed 
	//
	**************************************************************************************/
	void Draw(void)
	{
		// render the background
        BG->Render_Object(*M_BG);

        /*for (Floor& elem : floors)
        {
            elem.Render();
        }*/

		// render the floor
		for (Block& elem : blocks)
		{
			elem.Render();
		}
		
		// render the corresponding platforms depending on the current phase of the boss
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
		
		
		last_boss->Render(); // render king arthur on screen
        
        player->Render();    // render player on screen
         
        ui->Render();        // render the UI on screen
	}

	/**************************************************************************************
	//
	// Free the variables that were used
	//
	**************************************************************************************/
	void Free(void)
	{
		delete last_boss;
	}

	/**************************************************************************************
	//
	// Unloads the variables that were used
	//
	**************************************************************************************/
	void Unload(void)
	{
        // delete the map data;
		for (int y = 0; y < Map_Height; ++y)
        {
            delete[] MapData[y];
        }
        delete[] MapData;


		delete player;
		delete BG;
		delete M_BG;
		delete audio;
		delete ui;
		delete wall_sprite;
		delete floor_sprite;
		delete plat_sprite;
	}
}
