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
	Audio_Engine* Audio;
	//GameObject* Credits_Button;
	const float Menu_Width = AEGfxGetWinMaxX() * 2;
	const float Menu_Height = AEGfxGetWinMaxY() * 2;
	const float Button_Width = 80.0f;
	const float Button_Height = 40.0f;
	s32 Store_X ,Store_Y ;
	f32 Mouse_X, Mouse_Y ;

	Sprite* PLAY_SPRITE;
	Sprite* QUIT_SPRITE;
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

		// Construct Play Button 
		Play_Button = new GameObject{ PLAY_SPRITE,
			Col_Comp(0.0f -( Button_Width ), 0.0f - (Button_Height),
											0.0f + (Button_Width* 0.5),0.0f + (Button_Height * 0.5), Rect) };

		// Construct Quit Button 
		Quit_Button = new GameObject{ QUIT_SPRITE,
			Col_Comp(0.0f - (Button_Width ), -100.0f - (Button_Height),
				0.0f + (Button_Width), -100.0f + (Button_Height ), Rect) };

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
		Quit_Button->Transform_.SetTranslate(0.0f, -100.0f);
		Quit_Button->Transform_.Concat();

		Audio->Update();

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			AEInputGetCursorPosition(&Store_X, &Store_Y);
			Mouse_X = ( (float)Store_X -  AEGfxGetWinMaxX() );
			Mouse_Y =   AEGfxGetWinMaxY() - (float)Store_Y  ;
			if (Play_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				GSM::next = GS_LEVEL1_1;
			}
			if (Quit_Button->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				GSM::next = GS_QUIT;
			}

		}
		
	}

	void Draw(void)
	{
		MM_Background->Render_Object(*M_BG);
		Play_Button->Render();
		Play_Button -> Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		Quit_Button->Render();
		Quit_Button->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	}

	void Free(void) { 
		delete MM_Background;
		delete Play_Button;
		delete Quit_Button;
		delete Audio;
	}

	void Unload(void) {}
}