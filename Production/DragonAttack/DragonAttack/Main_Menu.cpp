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
#include <iostream>
namespace
{
	Sprite* MM_Background;
	Transform *M_BG;
	GameObject* Play_Button;
	GameObject* Quit_Button;
	GameObject* Credits_Button;
	Audio_Engine* Audio;
	//GameObject* Credits_Button;
	const float Menu_Width = AEGfxGetWinMaxX() * 2;
	const float Menu_Height = AEGfxGetWinMaxY() * 2;
	//The button's actual height and width is double of these values.
	//i.e, these are the half of button's height/width
	const float Button_Width = 80.0f;
	const float Button_Height = 40.0f;
	s32 Store_X ,Store_Y ;
	f32 Mouse_X, Mouse_Y ;
	 AEVec2 Play_Pos{ 0.0f, 0.0f };
	 AEVec2 Quit_Pos{ 0.0f, -100.0f };
	 AEVec2 Credits_Pos{ 0.0f, -200.0f };
	Sprite* PLAY_SPRITE;
	Sprite* QUIT_SPRITE;
	Sprite* CREDITS_SPRITE;
}

namespace Main_Menu
{
	void Load(void)
	{
		//AEToogleFullScreen(true);
		AEGfxSetCamPosition(0.0f, 0.0f);
		// BG constructed by using Move constructor 
		MM_Background = new Sprite{ CreateBG(1.0f, 1.0f, "Textures/Main_Menu_BG.png" ) };
		M_BG = new Transform{};

		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void{playlist.push_back(".//Audio/MainMenu_BGM.mp3"); } };

		PLAY_SPRITE = new Sprite{S_CreateRectangle(Button_Width,Button_Height,"Textures/Play_Button.png")};
		QUIT_SPRITE = new Sprite{S_CreateRectangle(Button_Width,Button_Height,"Textures/Quit_Button.png")};
		CREDITS_SPRITE = new Sprite{ S_CreateRectangle(Button_Width, Button_Height, "Textures/Credits_Button.png") };
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

	}

	void Init(void)
	{
		//Initialise buttons here
		Audio->Play(0);
		Audio->SetLoop(0,1);
	}

	void Update(float /*dt*/)
	{
		Play_Button->SetActive(true);
		Quit_Button->SetActive(true);
		Credits_Button->SetActive(true);
		//Translate and concat the play button
		Play_Button->Transform_.SetTranslate(Play_Pos.x, Play_Pos.y);
		Play_Button->Transform_.Concat();
		//Translate and concat the quit button
		Quit_Button->Transform_.SetTranslate(Quit_Pos.x, Quit_Pos.y);
		Quit_Button->Transform_.Concat();
		//Translate and concat the credits button
		Credits_Button->Transform_.SetTranslate(Credits_Pos.x, Credits_Pos.y);
		Credits_Button->Transform_.Concat();

		Audio->Update();

		//Use AE_Engine to get the cursor position and convert the position to the global scale 
		AEInputGetCursorPosition(&Store_X, &Store_Y);
		//Conversion needed as the position gotten from AEInputGetCursorPosition is not in Global System
		Mouse_X = ((float)Store_X - AEGfxGetWinMaxX());
		Mouse_Y = AEGfxGetWinMaxY() - (float)Store_Y;

		if (Play_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
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
		else  
		{
			PLAY_SPRITE->SetRGB(1.5f, 1.5f, 1.5f);
			QUIT_SPRITE->SetRGB(1.5f, 1.5f, 1.5f);
			CREDITS_SPRITE->SetRGB(1.5f, 1.5, 1.5f);
		}

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			

			//Use the Collision functions to check if the 'point'(mouse-click) is in the bouding box of the button
			//Repeat this for all buttons 
			if (Play_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				GSM::next = GS_LEVEL1_1;
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
		
	}

	void Draw(void)
	{
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
	}

	void Free(void) { 
		delete MM_Background;
		delete Play_Button;
		delete Quit_Button;
		delete Credits_Button;
		delete Audio;
	}

	void Unload(void) {}
}