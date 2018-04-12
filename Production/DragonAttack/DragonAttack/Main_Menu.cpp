/* Start Header ************************************************************************/
/*!
\file    Main_Menu.cpp
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
This is a game state that displays the Main Menu for the game

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Main_Menu.h"
#include "StageManager.h"
#include <iostream>
namespace
{
	Sprite* MM_Background;
	Transform *M_BG;
	GameObject* Instructions_Button;
	GameObject* Howtoplay;
	GameObject* Play_Button;
	GameObject* Quit_Button;
	GameObject* Credits_Button;
	GameObject* Yes_Button;
	GameObject* No_Button;
	GameObject* Cursor;
	Audio_Engine* Audio;
	GameObject* Options_Button;
	GameObject* Fullscreen_Button;
	GameObject* Mute_Button;
	//GameObject* Button_Borders;
	//GameObject* BB_Array []
	const float Menu_Width = AEGfxGetWinMaxX() * 2;
	const float Menu_Height = AEGfxGetWinMaxY() * 2;
	//The button's actual height and width is double of these values.
	//i.e, these are the half of button's height/width
	const float Button_Width = 80.0f;
	const float Button_Height = 40.0f;
	const float Popup_Width = 500.0f;
	const float Popup_Height = 300.0f;

	const float Cursor_D = 20.0f; // The dimensions of the height and width
	s32 Store_X ,Store_Y ;
	f32 Mouse_X, Mouse_Y ;
	AEVec2 Instructions_Pos	{ 0.0f, 100.0f };
	AEVec2 Howtoplay_Pos	{ 0.0f, -100.0f };
	AEVec2 Play_Pos			{ 0.0f, 0.0f };
	AEVec2 Options_Pos		{ 0.0f, -100.0f };
	AEVec2 Quit_Pos			{ 0.0f, -200.0f };
	AEVec2 Credits_Pos		{ 0.0f, -300.0f };
	AEVec2 Fullscreen_Pos	{ 0.0f, 0.0f };
	AEVec2 Mute_Pos			{ 0.0f, -100.0f };
	AEVec2 Cursor_Pos		{ 0.0f , 0.0f };
	AEVec2 Yes_Pos 			 { -100.0f , -100.0f };
	AEVec2 No_Pos			 { 100.0f , -100.0f };
	AEVec2 Confirm_Pos		 { 0.0f , 0.0f };

	Sprite* INSTRUCTIONS_SPRITE;
	Sprite* HOWTOPLAY_SPRITE;
	Sprite* PLAY_SPRITE;
	Sprite* QUIT_SPRITE;
	Sprite* CREDITS_SPRITE;
	Sprite* OPTIONS_SPRITE;
	Sprite* FULLSCREEN_SPRITE;
	Sprite* MUTE_SPRITE;
	Sprite* DIGIPEN_SPRITE;
	Sprite* CURSOR_SPRITE;
	Transform* DIGIPEN_M;
	Sprite* TEAM_SPRITE;
	Transform* TEAM_M;
	Sprite* GAME_SPRITE;

	Sprite* YES_SPRITE;
	Sprite* NO_SPRITE;
	Sprite* CONFIRM_SPRITE;
	Transform* CONFIRM_TRANS;

	Transform* GAME_M;
	Particle_System* cursor_particles;
	bool Instructions_Shown = false;
	bool Options_Shown = false;
	bool FS_active = true;
	bool Mute_active = false;
	bool Confirm_Shown = false;

	float timer = 15.0f;
	bool After_Load = false;
}

namespace Main_Menu
{
	void Load(void)
	{
		//Activate fullscreen at the start of the game 
		AEToogleFullScreen(FS_active);

		//set background color to black
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		
		//AEToogleFullScreen(true);
		AEGfxSetCamPosition(0.0f, 0.0f);
		// BG constructed by using Move constructor 
		MM_Background = new Sprite{ CreateBG(1.0f, 1.0f, "Textures/Main_Menu_BG.png") };
		M_BG = new Transform{};

		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/MainMenu_BGM.mp3"); } };

		INSTRUCTIONS_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/HowToPlay_Button.png") };
		HOWTOPLAY_SPRITE = new Sprite{ S_CreateRectangle(Popup_Width,Popup_Height,"Textures/HowToPlay.png") };


		PLAY_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Play_Button.png") };
		QUIT_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Quit_Button.png") };
		CREDITS_SPRITE = new Sprite{ S_CreateRectangle(Button_Width, Button_Height, "Textures/Credits_Button.png") };

		CURSOR_SPRITE = new Sprite{ S_CreateRectangle(Cursor_D, Cursor_D, "Textures/Bob_Head.png") };

		OPTIONS_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Options_Button.png") };
		FULLSCREEN_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Fullscreen_Button.png") };
		MUTE_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Mute_Button.png") };

		YES_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Yes.png") };
		NO_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/No.png") };
		CONFIRM_SPRITE = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/AreYouSure.png") };
		CONFIRM_TRANS = new Transform;

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

		//Construct Instructions Button
		Instructions_Button = new GameObject{ INSTRUCTIONS_SPRITE,
			Col_Comp(Instructions_Pos.x - Button_Width , Instructions_Pos.y - (Button_Height),
				Instructions_Pos.x + Button_Width, Instructions_Pos.y + (Button_Height), Rect) };

		//Construct Instructions Pop_Up
		Howtoplay = new GameObject{ HOWTOPLAY_SPRITE,
			Col_Comp(Howtoplay_Pos.x - Popup_Width , Howtoplay_Pos.y - (Popup_Height),
				Howtoplay_Pos.x + Popup_Width, Howtoplay_Pos.y + (Popup_Height), Rect) };

		// Construct Play Button 
		Play_Button = new GameObject{ PLAY_SPRITE,
			Col_Comp(Play_Pos.x - Button_Width, Play_Pos.y - Button_Height,
				Play_Pos.x + Button_Width, Play_Pos.y + Button_Height , Rect) };

		// Construct Quit Button 
		Quit_Button = new GameObject{ QUIT_SPRITE,
			Col_Comp(Quit_Pos.x - Button_Width , Quit_Pos.y - (Button_Height),
				Quit_Pos.x + Button_Width, Quit_Pos.y + (Button_Height), Rect) };

		// Construct Credits Button 
		Credits_Button = new GameObject{ CREDITS_SPRITE,
			Col_Comp(Credits_Pos.x - (Button_Width), Credits_Pos.y - Button_Height,
				Credits_Pos.x + Button_Width, Credits_Pos.y + Button_Height, Rect) };

		//Construct Options Button
		Options_Button = new GameObject{ OPTIONS_SPRITE,
			Col_Comp(Options_Pos.x - (Button_Width), Options_Pos.y - Button_Height,
				Options_Pos.x + Button_Width, Options_Pos.y + Button_Height, Rect) };

		//Construct Fullscreen Button
		Fullscreen_Button = new GameObject{ FULLSCREEN_SPRITE,
			Col_Comp(Fullscreen_Pos.x - (Button_Width), Fullscreen_Pos.y - Button_Height,
				Fullscreen_Pos.x + Button_Width, Fullscreen_Pos.y + Button_Height, Rect) };

		//Construct Mute Button 
		Mute_Button = new GameObject{ MUTE_SPRITE,
			Col_Comp(Mute_Pos.x - (Button_Width), Mute_Pos.y - Button_Height,
				Mute_Pos.x + Button_Width, Mute_Pos.y + Button_Height, Rect) };

		//Construct Yes Button 
		Yes_Button = new GameObject{ YES_SPRITE,
			Col_Comp(Yes_Pos.x - (Button_Width), Yes_Pos.y - Button_Height,
				Yes_Pos.x + Button_Width, Yes_Pos.y + Button_Height, Rect) };

		//Construct No Button 
		No_Button = new GameObject{ NO_SPRITE,
			Col_Comp(No_Pos.x - (Button_Width), No_Pos.y - Button_Height,
				No_Pos.x + Button_Width, No_Pos.y + Button_Height, Rect) };

		//Construct a custom Cursor 
		Cursor = new GameObject{ CURSOR_SPRITE, Col_Comp() };

		cursor_particles = Effects_Get(CURSOR_PARTICLE);

	}

	void Init(void)
	{
		//Initialise buttons here
		Audio->Play(0);
		Audio->SetLoop(0, FMOD_LOOP_NORMAL);

		// pause the music and set volume to 0 if current state is muted
		if (Audio_Engine::MUTE_)
		{
			Audio->SetVolume(0, 0.0f); // set volume to 0
			Audio->SetPause(0, true);  // pause volume
		}

		//Only need to Translate and Concat the buttons once for Main Menu

		//Translate and concat the Instructions Button 
		Instructions_Button->Transform_.SetTranslate(Instructions_Pos.x, Instructions_Pos.y);
		Instructions_Button->Transform_.Concat();
		Instructions_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the HowToPlay POP-UP
		Howtoplay->Transform_.SetTranslate(Howtoplay_Pos.x, Howtoplay_Pos.y);
		Howtoplay->Transform_.Concat();
		Howtoplay->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the play button 
		Play_Button->Transform_.SetTranslate(Play_Pos.x, Play_Pos.y);
		Play_Button->Transform_.Concat();
		Play_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the quit button
		Quit_Button->Transform_.SetTranslate(Quit_Pos.x, Quit_Pos.y);
		Quit_Button->Transform_.Concat();
		Quit_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the credits button
		Credits_Button->Transform_.SetTranslate(Credits_Pos.x, Credits_Pos.y);
		Credits_Button->Transform_.Concat();
		Credits_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the options button
		Options_Button->Transform_.SetTranslate(Options_Pos.x, Options_Pos.y);
		Options_Button->Transform_.Concat();
		Options_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the fullscreen button
		Fullscreen_Button->Transform_.SetTranslate(Fullscreen_Pos.x, Fullscreen_Pos.y);
		Fullscreen_Button->Transform_.Concat();
		Fullscreen_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the mute button
		Mute_Button->Transform_.SetTranslate(Mute_Pos.x, Mute_Pos.y);
		Mute_Button->Transform_.Concat();
		Mute_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the Yes button
		Yes_Button->Transform_.SetTranslate(Yes_Pos.x, Yes_Pos.y);
		Yes_Button->Transform_.Concat();
		Yes_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the Yes button
		No_Button->Transform_.SetTranslate(No_Pos.x, No_Pos.y);
		No_Button->Transform_.Concat();
		No_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//Translate and concat the transformation matrix for Confirmation
		CONFIRM_TRANS->SetTranslate(Confirm_Pos.x, Confirm_Pos.y);
		CONFIRM_TRANS->Concat();
		CONFIRM_SPRITE->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);


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
		cursor_particles->Emitter_.Lifetime_ = 0.17f;
		//cursor_particles->Emitter_.Particle_Rand_.Life_Rand_ = 3;
	}

	void Update(float dt)
	{
		Audio->Update();
		
		//Use AE_Engine to get the cursor position and convert the position to the global scale 
		AEInputGetCursorPosition(&Store_X, &Store_Y);
		//Conversion needed as the position gotten from AEInputGetCursorPosition is not in Global System
		Mouse_X = ((float)Store_X - AEGfxGetWinMaxX());
		Mouse_Y = AEGfxGetWinMaxY() - (float)Store_Y;
		Cursor_Pos.x = Mouse_X + 10.0f;
		Cursor_Pos.y = Mouse_Y - 10.0f;

		if (AEInputCheckTriggered(AEVK_LBUTTON))// && timer == 0.0f )
		{
			if (After_Load)
			{
				//Use the Collision functions to check if the 'point'(mouse-click) is in the bouding box of the button
				//Repeat this for all buttons
				if (!Options_Shown && !Instructions_Shown && !Confirm_Shown)
				{
					if (Instructions_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						Instructions_Shown = Instructions_Shown ? false : true;
					}

					if (Play_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						//SM::Reset();
						GSM::next = GS_LEVEL_SELECTOR;
					}

					if (Quit_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						Confirm_Shown = Confirm_Shown ? false : true;
					}

					if (Credits_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						GSM::next = GS_CREDITS;
					}

					if (Options_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						Options_Shown = Options_Shown ? false : true;
					}
				}

				else if (Options_Shown)
				{
					if (Fullscreen_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						FS_active = FS_active ? false : true;
						AEToogleFullScreen(FS_active);
					}

					if (Mute_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						Audio_Engine::MUTE_ = Audio_Engine::MUTE_ ? false : true; 
					}
				}

				else if (Confirm_Shown)
				{
					if (Yes_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						GSM::next = GS_QUIT;
					}
					if (No_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
					{
						Confirm_Shown = Confirm_Shown ? false : true;
					}
				}
				if (Audio_Engine::MUTE_)
				{
					Audio->SetVolume(0, 0);
				}
				else
				{
					Audio->SetVolume(0, 1);
				}
			}
		}

		if ( !Options_Shown && !Confirm_Shown && !Instructions_Shown &&
			AEInputCheckTriggered(AEVK_ESCAPE)) Confirm_Shown = true;

			//******Allowing Intro screen to be bypassed with a single mouse-click,ESC,ENTER or space
		if (!After_Load) 
		{
			if (AEInputCheckTriggered(AEVK_ESCAPE) || AEInputCheckTriggered(AEVK_SPACE) ||
				AEInputCheckTriggered(AEVK_RETURN) || AEInputCheckReleased(AEVK_LBUTTON))
			{
				After_Load = true;
				timer = 0.0f;
			}

			if (timer > 0.0f)
			{
				timer -= dt;
				return;
			}
		}


			if (Options_Shown)
			{
				Instructions_Button->		SetActive(false);
				Play_Button->				SetActive(false);
				Quit_Button->				SetActive(false);
				Credits_Button->			SetActive(false);
				Options_Button->			SetActive(false);
				if (AEInputCheckTriggered(AEVK_ESCAPE))
				{
					Options_Shown = Options_Shown ? false : true;
				}
				if (Fullscreen_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
				{
					FULLSCREEN_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
				}
				else if (Mute_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
				{
					MUTE_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
				}
				else
				{
					FULLSCREEN_SPRITE->SetRGB(1.5f, 1.5f, 1.5f);
					MUTE_SPRITE->SetRGB(1.5f, 1.5, 1.5f);
				}
			}

			else if (Instructions_Shown)
			{
				Instructions_Button->		SetActive(false);
				Play_Button->				SetActive(false);
				Quit_Button->				SetActive(false);
				Credits_Button->			SetActive(false);
				Options_Button->			SetActive(false);
				if (AEInputCheckTriggered(AEVK_ESCAPE))
				{
					Instructions_Shown = Instructions_Shown ? false : true;
				}
			}

			else if (Confirm_Shown)
			{
				Instructions_Button->SetActive(false);
				Play_Button->SetActive(false);
				Quit_Button->SetActive(false);
				Credits_Button->SetActive(false);
				Options_Button->SetActive(false);
				if (AEInputCheckTriggered(AEVK_ESCAPE)) Confirm_Shown = Confirm_Shown ? false : true;
			}

			else if (!Options_Shown && !Instructions_Shown)
			{
				Instructions_Button->		SetActive(true);
				Play_Button->				SetActive(true);
				Quit_Button->				SetActive(true);
				Credits_Button->			SetActive(true);
				Options_Button->			SetActive(true);
			}

			Cursor->SetActive(true);

			Fullscreen_Button->SetActive(Options_Shown);
			Mute_Button->SetActive(Options_Shown);

			Howtoplay->SetActive(Instructions_Shown);

			Yes_Button->SetActive(Confirm_Shown);
			No_Button->SetActive(Confirm_Shown);

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
			if (Instructions_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				INSTRUCTIONS_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
			}
			else if (Play_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				PLAY_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
			}

			else if (Quit_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				QUIT_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
			}

			else if (Credits_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				CREDITS_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
			}
			else if (Options_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				OPTIONS_SPRITE->SetRGB(1.0f, 1.0f, 1.0f);
			}

			

			else if (Yes_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				YES_SPRITE->SetRGB(0.7f, 0.7f, 0.7f);
			}
			else if (No_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				NO_SPRITE->SetRGB(0.7f, 0.7f, 0.7f);
			}

			else
			{
				INSTRUCTIONS_SPRITE->		SetRGB(1.5f, 1.5f, 1.5f);
				PLAY_SPRITE->				SetRGB(1.5f, 1.5f, 1.5f);
				QUIT_SPRITE->				SetRGB(1.5f, 1.5f, 1.5f);
				CREDITS_SPRITE->			SetRGB(1.5f, 1.5, 1.5f);
				OPTIONS_SPRITE->			SetRGB(1.5f, 1.5f, 1.5f);
				
				YES_SPRITE->				SetRGB(1.0f, 1.0f, 1.0f);
				NO_SPRITE->					SetRGB(1.0f, 1.0f, 1.0f);
			}



		
	} // END OF UPDATE FUNCTIONS 

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
		if (Confirm_Shown)
		CONFIRM_SPRITE->Render_Object(*CONFIRM_TRANS);

		Instructions_Button->	Render();
		Howtoplay->				Render();
		Play_Button->			Render();//Render the Play button 
		Quit_Button->			Render();//Render the Quit button 
		Credits_Button->		Render();//Render the Credits button 
		Options_Button->		Render();//Render Options
		Fullscreen_Button->		Render();//Render Fullscreen
		Mute_Button->			Render();//Render Mute 
		Yes_Button->			Render();
		No_Button->				Render();
		Cursor->Render();
		Cursor->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		cursor_particles->Render();

	}

	void Free(void) { 
		delete INSTRUCTIONS_SPRITE;
		delete HOWTOPLAY_SPRITE;
		delete PLAY_SPRITE;
		delete QUIT_SPRITE;
		delete CREDITS_SPRITE;
		delete OPTIONS_SPRITE;
		delete FULLSCREEN_SPRITE;
		delete MUTE_SPRITE;
		delete DIGIPEN_SPRITE;
		delete DIGIPEN_M;
		delete TEAM_SPRITE;
		delete TEAM_M;
		delete GAME_SPRITE;
		delete CURSOR_SPRITE;
		delete YES_SPRITE;
		delete NO_SPRITE;
		delete CONFIRM_SPRITE;
		delete CONFIRM_TRANS;
		delete GAME_M;
		delete M_BG;
		delete MM_Background;
		delete Instructions_Button;
		delete Howtoplay;
		delete Play_Button;
		delete Quit_Button;
		delete Credits_Button;
		delete Options_Button;
		delete Fullscreen_Button;
		delete Mute_Button;
		delete No_Button;
		delete Yes_Button;
		delete Cursor;
		delete Audio;

	}

	void Unload(void) {}
}