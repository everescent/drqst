/* Start Header ************************************************************************/
/*!
\file       Main_Menu.cpp
\author     Javon Lee
\par email: xiongweijavon.lee\@digipen.edu
\brief
The Main menu (gamestate) functions definition are done here. 

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Main_Menu.h"
#include "StageManager.h"
#include <iostream>
namespace
{
	Sprite* MM_Background;
	Transform *M_BG;
	GameObject* Play_Button;
	GameObject* Quit_Button;
	GameObject* Credits_Button;
	GameObject* Cursor;
	Audio_Engine* Audio;
	//GameObject* Credits_Button;
	const float Menu_Width = AEGfxGetWinMaxX() * 2;
	const float Menu_Height = AEGfxGetWinMaxY() * 2;
	//The button's actual height and width is double of these values.
	//i.e, these are the half of button's height/width
	const float Button_Width = 80.0f;
	const float Button_Height = 40.0f;
	const float Cursor_D = 20.0f; // The dimensions of the height and width
	s32 Store_X ,Store_Y ;
	f32 Mouse_X, Mouse_Y ;
	 AEVec2 Play_Pos{ 0.0f, 0.0f };
	 AEVec2 Quit_Pos{ 0.0f, -100.0f };
	 AEVec2 Credits_Pos{ 0.0f, -200.0f };
	 AEVec2 Cursor_Pos{ 0.0f , 0.0f };
	Sprite* PLAY_SPRITE;
	Sprite* QUIT_SPRITE;
	Sprite* CREDITS_SPRITE;
	Sprite* DIGIPEN_SPRITE;
	Sprite* CURSOR_SPRITE;
	Transform* DIGIPEN_M;
	Sprite* TEAM_SPRITE;
	Transform* TEAM_M;
	Sprite* GAME_SPRITE;
	Transform* GAME_M;
	Particle_System* cursor_particles;


	float timer = 15.0f;
}

namespace Main_Menu
{
	void Load(void)
	{
		//set background color to black
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		
		//AEToogleFullScreen(true);
		AEGfxSetCamPosition(0.0f, 0.0f);
		// BG constructed by using Move constructor 
		MM_Background = new Sprite{ CreateBG(1.0f, 1.0f, "Textures/Main_Menu_BG.png" ) };
		M_BG = new Transform{};

		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void{playlist.push_back(".//Audio/MainMenu_BGM.mp3"); } };

		PLAY_SPRITE = new Sprite{S_CreateRectangle(Button_Width,Button_Height,"Textures/Play_Button.png")};
		QUIT_SPRITE = new Sprite{S_CreateRectangle(Button_Width,Button_Height,"Textures/Quit_Button.png")};
		CREDITS_SPRITE = new Sprite{ S_CreateRectangle(Button_Width, Button_Height, "Textures/Credits_Button.png") };
		
		CURSOR_SPRITE = new Sprite{ S_CreateRectangle(Cursor_D, Cursor_D, "Textures/Bob_Head.png") };
		
		DIGIPEN_SPRITE = new Sprite{ S_CreateRectangle(210.f, 50.f, "Textures/DigiPen_RGB_Red.png") };
		DIGIPEN_SPRITE->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		DIGIPEN_M = new Transform;
		DIGIPEN_M->SetScale(2.f, 2.f);
		DIGIPEN_M->Concat();

		TEAM_SPRITE = new Sprite{ S_CreateRectangle(64.f, 40.f, "Textures/TeamName.png") };
		TEAM_SPRITE->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		TEAM_M = new Transform;
		TEAM_M->SetScale(8.f, 8.f);
		TEAM_M->Concat();

		GAME_SPRITE = new Sprite{ S_CreateRectangle(64.f, 32.f, "Textures/GameTitle.png") };
		GAME_SPRITE->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		GAME_M = new Transform;
		GAME_M->SetScale(8.f, 8.f);
		GAME_M->Concat();

		// Construct Play Button 
		Play_Button = new GameObject{ PLAY_SPRITE,
			Col_Comp(Play_Pos.x - Button_Width, Play_Pos.y - Button_Height,
				Play_Pos.x + Button_Width, Play_Pos.y + Button_Height , Rect) };

		// Construct Quit Button 
		Quit_Button = new GameObject{ QUIT_SPRITE,
			Col_Comp(Quit_Pos.x - Button_Width , Quit_Pos.y - (Button_Height),
				Quit_Pos.x + Button_Width, Quit_Pos.y + (Button_Height ), Rect) } ;

		// Construct Credits Button 
		Credits_Button = new GameObject{ CREDITS_SPRITE,
			Col_Comp(Credits_Pos.x - (Button_Width), Credits_Pos.y - Button_Height,
				Credits_Pos.x + Button_Width, Credits_Pos.y + Button_Height, Rect) } ;

		//Construct a custom Cursor 
		Cursor = new GameObject{ CURSOR_SPRITE, Col_Comp() };

		cursor_particles = Effects_Get(COIN_PARTICLE);

	}

	void Init(void)
	{
		//Initialise buttons here
		Audio->Play(0);
		Audio->SetLoop(0,1);

		// BEHAVIOUR FOR CURSOR PARTICLES
		//Set to appear to be spitting out flames
		cursor_particles->Emitter_.PPS_ = 5;
		cursor_particles->Emitter_.Dist_Min_ = 0.0f;
		cursor_particles->Emitter_.Vol_Max = 100;
		cursor_particles->Emitter_.Direction_ = 0.0f; // shoot out to the right 
		cursor_particles->Emitter_.Particle_Rand_.Spread_ = 10;
		cursor_particles->Emitter_.Conserve_ = 0.80f;
		cursor_particles->Emitter_.Size_ = 8.0f;
		cursor_particles->Emitter_.Speed_ = 8.0f;
		//cursor_particles->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
		cursor_particles->Emitter_.Lifetime_ = 0.5f;
		//cursor_particles->Emitter_.Particle_Rand_.Life_Rand_ = 3;
	}

	void Update(float dt)
	{
		
		Audio->Update();
		AEInputShowCursor(0);
		//Use AE_Engine to get the cursor position and convert the position to the global scale 
		AEInputGetCursorPosition(&Store_X, &Store_Y);
		//Conversion needed as the position gotten from AEInputGetCursorPosition is not in Global System
		Mouse_X = ((float)Store_X - AEGfxGetWinMaxX());
		Mouse_Y = AEGfxGetWinMaxY() - (float)Store_Y;
		Cursor_Pos.x = Mouse_X + 10.0f;
		Cursor_Pos.y = Mouse_Y - 10.0f;


		//******Allowing Intro screen to be bypassed with a single mouse-click
		if (AEInputCheckTriggered(AEVK_ESCAPE) || AEInputCheckTriggered(AEVK_SPACE) ||
			AEInputCheckTriggered(AEVK_RETURN))
		{
			timer = 0.0f;
		}

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{

			timer = 0.0f;

			//Use the Collision functions to check if the 'point'(mouse-click) is in the bouding box of the button
			//Repeat this for all buttons 
			if (Play_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
                SM::Reset(STAGE_2_3);
                GSM::next = GS_LEVELS;
			}

			if (Quit_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				GSM::next = GS_QUIT;
			}

			if (Credits_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				GSM::next = GS_CREDITS;
			}



		}
		
		if (timer > 0.0f)
		{
			timer -= dt;
			return;
		}
		
		
		Play_Button->SetActive(true);
		Quit_Button->SetActive(true);
		Credits_Button->SetActive(true);
		Cursor->SetActive(true);
		//Translate and concat the play button
		Play_Button->Transform_.SetTranslate(Play_Pos.x, Play_Pos.y);
		Play_Button->Transform_.Concat();
		//Translate and concat the quit button
		Quit_Button->Transform_.SetTranslate(Quit_Pos.x, Quit_Pos.y);
		Quit_Button->Transform_.Concat();
		//Translate and concat the credits button
		Credits_Button->Transform_.SetTranslate(Credits_Pos.x, Credits_Pos.y);
		Credits_Button->Transform_.Concat();

		Cursor->Transform_.SetTranslate(Cursor_Pos.x, Cursor_Pos.y);
		Cursor->Transform_.Concat();

		

		cursor_particles->Emitter_.Pos_.Min_Max.Point_Max.x = Cursor_Pos.x + 480.0f;
		cursor_particles->Emitter_.Pos_.Min_Max.Point_Min.x = Cursor_Pos.x + 10.0f;
		cursor_particles->Emitter_.Pos_.Min_Max.Point_Max.y = Cursor_Pos.y + 10.0f;
		cursor_particles->Emitter_.Pos_.Min_Max.Point_Min.y = Cursor_Pos.y - 10.0f;

		cursor_particles->UpdateEmission();
		cursor_particles->Turbulence(0.7f);
		cursor_particles->TransRamp_Exp();
		cursor_particles->Newton({ Cursor_Pos.x + 20.0f , Cursor_Pos.y - 10.0f }, 0.2f);

		if (cursor_particles->GetParticleCount())
		{
			cursor_particles->Update(dt);
		}

		if (Play_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			PLAY_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
			//Will add in particle effects later 
		}

		else if (Quit_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			QUIT_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
		}

		else if (Credits_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			CREDITS_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
		}
		else  
		{
			PLAY_SPRITE->SetRGB(1.5f, 1.5f, 1.5f);
			QUIT_SPRITE->SetRGB(1.5f, 1.5f, 1.5f);
			CREDITS_SPRITE->SetRGB(1.5f, 1.5, 1.5f);
		}

		
		
	}

	void Draw(void)
	{
		static float Exp_[3] = { 1.0f, 1.0f, 1.0f };
		if (timer > 10.0f)
		{
			DIGIPEN_SPRITE->Render_Object(*DIGIPEN_M);
			DIGIPEN_SPRITE->SetRGB(Exp_[0], Exp_[0], Exp_[0]);
			if(timer <= 12.5f)
				Exp_[0] -= 0.008f;
			return;
		}
		else if(timer > 5.0f)
		{
			TEAM_SPRITE->Render_Object(*TEAM_M);
			TEAM_SPRITE->SetRGB(Exp_[1], Exp_[1], Exp_[1]);
			if (timer <= 7.5f)
				Exp_[1] -= 0.008f;
			return;
		}
		else if(timer > 0.0f)
		{
			GAME_SPRITE->Render_Object(*GAME_M);
			GAME_SPRITE->SetRGB(Exp_[2], Exp_[2], Exp_[2]);
			if (timer <= 2.5f)
				Exp_[2] -= 0.008f;
			return;
		}
		
		//Always render the background image first  
		MM_Background->Render_Object(*M_BG);
		//Render the Play button 
		Play_Button->Render();
		Play_Button -> Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Render the Quit button 
		Quit_Button->Render();
		Quit_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Render the Credits button 
		Credits_Button->Render();
		Credits_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Cursor->Render();
		Cursor->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		cursor_particles->Render();

	}

	void Free(void) { 
		delete PLAY_SPRITE;
		delete QUIT_SPRITE;
		delete CREDITS_SPRITE;
		delete DIGIPEN_SPRITE;
		delete DIGIPEN_M;
		delete TEAM_SPRITE;
		delete TEAM_M;
		delete GAME_SPRITE;
		delete CURSOR_SPRITE;
		delete GAME_M;
		delete M_BG;
		delete MM_Background;
		delete Play_Button;
		delete Quit_Button;
		delete Credits_Button;
		delete Cursor;
		delete Audio;

	}

	void Unload(void) {}
}