/* Start Header ************************************************************************/
/*!
\file       Level_Selector.cpp
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
#include "Level_Selector.h"
namespace
{
	Sprite* MM_Background;
	Transform *M_BG;

	GameObject* Stage_1_1;
	GameObject* Stage_1_2;
	GameObject* Stage_1_3;

	GameObject* Stage_2_1;
	GameObject* Stage_2_2; 
	GameObject* Stage_2_3;

	GameObject* Stage_3_1;
	GameObject* Stage_3_2;
	GameObject* Stage_3_3;
	GameObject* Cursor;
	Audio_Engine* Audio;
	const float Menu_Width = AEGfxGetWinMaxX() * 2;
	const float Menu_Height = AEGfxGetWinMaxY() * 2;
	//The button's actual height and width is double of these values.
	//i.e, these are the half of button's height/width
	const float Button_Width = 80.0f;
	const float Button_Height = 40.0f;
	const float Cursor_D = 20.0f; // The dimensions of the height and width
	s32 Store_X, Store_Y;
	f32 Mouse_X, Mouse_Y;
	AEVec2 Stage_1_1_pos { -200.0f, 0.0f };
	AEVec2 Stage_1_2_pos{ -200.0f, -100.0f };
	AEVec2 Stage_1_3_pos{ -200.0f, -200.0f };

	AEVec2 Stage_2_1_pos{ 0.0f, 0.0f };
	AEVec2 Stage_2_2_pos{ 0.0f, -100.0f };
	AEVec2 Stage_2_3_pos{ 0.0f, -200.0f };

	AEVec2 Stage_3_1_pos{ 200.0f, 0.0f };
	AEVec2 Stage_3_2_pos{ 200.0f, -100.0f };
	AEVec2 Stage_3_3_pos{ 200.0f, -200.0f };

	AEVec2 Cursor_Pos{ 0.0f , 0.0f };
	 
	Sprite* Stage_1_1_Sprite;
	Sprite* Stage_1_2_Sprite;
	Sprite* Stage_1_3_Sprite;
			
	Sprite* Stage_2_1_Sprite;
	Sprite* Stage_2_2_Sprite;
	Sprite* Stage_2_3_Sprite;

	Sprite* Stage_3_1_Sprite;
	Sprite* Stage_3_2_Sprite;
	Sprite* Stage_3_3_Sprite;

	Sprite* CURSOR_SPRITE;

	Particle_System* cursor_particles;

	float timer = 15.0f;
}

namespace Level_Selector
{
	void Load(void)
	{
		//set background color to black
		//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		//AEToogleFullScreen(true);
		AEGfxSetCamPosition(0.0f, 0.0f);
		// BG constructed by using Move constructor 
		MM_Background = new Sprite{ CreateBG(1.0f, 1.0f, "Textures/Main_Menu_BG.png") };
		M_BG = new Transform{};

		Audio = new Audio_Engine{ 1, [](std::vector <std::string> &playlist)->void {playlist.push_back(".//Audio/MainMenu_BGM.mp3"); } };

		Stage_1_1_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage11_Button.png") };
		Stage_1_2_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage12_Button.png") };
		Stage_1_3_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage13_Button.png") };

		Stage_2_1_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage21_Button.png") };
		Stage_2_2_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage22_Button.png") };
		Stage_2_3_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage23_Button.png") };

		Stage_3_1_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage31_Button.png") };
		Stage_3_2_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage32_Button.png") };
		Stage_3_3_Sprite = new Sprite{ S_CreateRectangle(Button_Width,Button_Height,"Textures/Stage33_Button.png") };



		CURSOR_SPRITE = new Sprite{ S_CreateRectangle(Cursor_D, Cursor_D, "Textures/Bob_Head.png") };

		Stage_1_1 =  new GameObject{ Stage_1_1_Sprite,
			Col_Comp(Stage_1_1_pos.x - Button_Width,Stage_1_1_pos.y - Button_Height,
				Stage_1_1_pos.x + Button_Width, Stage_1_1_pos.y + Button_Height, Rect) };

		Stage_1_2 = new GameObject{ Stage_1_2_Sprite,
			Col_Comp(Stage_1_2_pos.x - Button_Width,Stage_1_2_pos.y - Button_Height,
				Stage_1_2_pos.x + Button_Width, Stage_1_2_pos.y + Button_Height, Rect) };

		Stage_1_3 = new GameObject{ Stage_1_3_Sprite,
			Col_Comp(Stage_1_3_pos.x - Button_Width,Stage_1_3_pos.y - Button_Height,
				Stage_1_3_pos.x + Button_Width, Stage_1_3_pos.y + Button_Height, Rect) };

		Stage_2_1 = new GameObject{ Stage_2_1_Sprite,
			Col_Comp(Stage_2_1_pos.x - Button_Width,Stage_2_1_pos.y - Button_Height,
				Stage_2_1_pos.x + Button_Width, Stage_2_1_pos.y + Button_Height, Rect) };

		Stage_2_2 = new GameObject{ Stage_2_2_Sprite,
			Col_Comp(Stage_2_2_pos.x - Button_Width,Stage_2_2_pos.y - Button_Height,
				Stage_2_2_pos.x + Button_Width, Stage_2_2_pos.y + Button_Height, Rect) };

		Stage_2_3 = new GameObject{ Stage_2_3_Sprite,
			Col_Comp(Stage_2_3_pos.x - Button_Width,Stage_2_3_pos.y - Button_Height,
				Stage_2_3_pos.x + Button_Width, Stage_2_3_pos.y + Button_Height, Rect) };

		Stage_3_1 = new GameObject{ Stage_3_1_Sprite,
			Col_Comp(Stage_3_1_pos.x - Button_Width,Stage_3_1_pos.y - Button_Height,
				Stage_3_1_pos.x + Button_Width, Stage_3_1_pos.y + Button_Height, Rect) };

		Stage_3_2 = new GameObject{ Stage_3_2_Sprite,
			Col_Comp(Stage_3_2_pos.x - Button_Width,Stage_3_2_pos.y - Button_Height,
				Stage_3_2_pos.x + Button_Width, Stage_3_2_pos.y + Button_Height, Rect) };

		Stage_3_3 = new GameObject{ Stage_3_3_Sprite,
			Col_Comp(Stage_3_3_pos.x - Button_Width,Stage_3_3_pos.y - Button_Height,
				Stage_3_3_pos.x + Button_Width, Stage_3_3_pos.y + Button_Height, Rect) };

		//Construct a custom Cursor 
		Cursor = new GameObject{ CURSOR_SPRITE, Col_Comp() };

		cursor_particles = Effects_Get(CURSOR_PARTICLE);

		Stage_1_1->Transform_.SetTranslate(Stage_1_1_pos.x, Stage_1_1_pos.y);
		Stage_1_1->Transform_.Concat();
		Stage_1_2->Transform_.SetTranslate(Stage_1_2_pos.x, Stage_1_2_pos.y);
		Stage_1_2->Transform_.Concat();
		Stage_1_3->Transform_.SetTranslate(Stage_1_3_pos.x, Stage_1_3_pos.y);
		Stage_1_3->Transform_.Concat();

		Stage_2_1->Transform_.SetTranslate(Stage_2_1_pos.x, Stage_2_1_pos.y);
		Stage_2_1->Transform_.Concat();
		Stage_2_2->Transform_.SetTranslate(Stage_2_2_pos.x, Stage_2_2_pos.y);
		Stage_2_2->Transform_.Concat();
		Stage_2_3->Transform_.SetTranslate(Stage_2_3_pos.x, Stage_2_3_pos.y);
		Stage_2_3->Transform_.Concat();

		Stage_3_1->Transform_.SetTranslate(Stage_3_1_pos.x, Stage_3_1_pos.y);
		Stage_3_1->Transform_.Concat();
		Stage_3_2->Transform_.SetTranslate(Stage_3_2_pos.x, Stage_3_2_pos.y);
		Stage_3_2->Transform_.Concat();
		Stage_3_3->Transform_.SetTranslate(Stage_3_3_pos.x, Stage_3_3_pos.y);
		Stage_3_3->Transform_.Concat();


		Stage_1_1->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_1_2->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_1_3->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_2_1->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_2_2->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_2_3->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_3_1->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_3_2->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		Stage_3_3->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	}

	void Init(void)
	{
		//Initialise buttons here
		Audio->Play(0);
		Audio->SetLoop(0, FMOD_LOOP_NORMAL);

		 //BEHAVIOUR FOR CURSOR PARTICLES
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

		Stage_1_1->SetActive(true);
		Stage_1_2->SetActive(true);
		Stage_1_3->SetActive(true);

		Stage_2_1->SetActive(true);
		Stage_2_2->SetActive(true);
		Stage_2_3->SetActive(true);

		Stage_3_1->SetActive(true);
		Stage_3_2->SetActive(true);
		Stage_3_3->SetActive(true);

		Cursor->SetActive(true);

	}

	void Update(float dt)
	{
		UNREFERENCED_PARAMETER (dt);
		Audio->Update();
		AEInputShowCursor(0);
		//Use AE_Engine to get the cursor position and convert the position to the global scale 
		AEInputGetCursorPosition(&Store_X, &Store_Y);
		//Conversion needed as the position gotten from AEInputGetCursorPosition is not in Global System
		Mouse_X = ((float)Store_X - AEGfxGetWinMaxX());
		Mouse_Y = AEGfxGetWinMaxY() - (float)Store_Y;
		Cursor_Pos.x = Mouse_X + 10.0f;
		Cursor_Pos.y = Mouse_Y - 10.0f;

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{

			//Use the Collision functions to check if the 'point'(mouse-click) is in the bouding box of the button
			//Repeat this for all buttons 
			if (Stage_1_1->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_1_1);
				GSM::next = GS_LEVELS;
			}

			if (Stage_1_2->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_1_2);
				GSM::next = GS_LEVELS;
			}

			if (Stage_1_3->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_1_3);
				GSM::next = GS_LEVELS;
			}

			//////////////////////// Stage 2 ////////////////////////////////////////////

			if (Stage_2_1->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_2_1);
				GSM::next = GS_LEVELS;
			}

			if (Stage_2_2->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_2_2);
				GSM::next = GS_LEVELS;
			}

			if (Stage_2_3->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_2_3);
				GSM::next = GS_LEVELS;
			}

			//////////////////////// Stage 3 ////////////////////////////////////////////

			if (Stage_3_1->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_3_1);
				GSM::next = GS_LEVELS;
			}

			if (Stage_3_2->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_3_2);
				GSM::next = GS_LEVELS;
			}

			if (Stage_3_3->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
			{
				SM::Reset(STAGE_3_3);
				GSM::next = GS_LEVELS;
			}

		}

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

		if (Stage_1_1->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_1_1_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		else if (Stage_1_2->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_1_2_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		else if (Stage_1_3->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_1_3_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		//////////////////////// Stage 2 ////////////////////////////////////////////

		else if (Stage_2_1->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_2_1_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		else if (Stage_2_2->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_2_2_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		else if (Stage_2_3->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_2_3_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		//////////////////////// Stage 3 ////////////////////////////////////////////

		else if (Stage_3_1->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_3_1_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		else if (Stage_3_2->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_3_2_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}

		else if (Stage_3_3->Collision_.St_Rect_Point((float)Mouse_X, (float)Mouse_Y))
		{
			Stage_3_3_Sprite->SetRGB(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Stage_1_1_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_1_2_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_1_3_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_2_1_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_2_2_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_2_3_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_3_1_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_3_2_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
			Stage_3_3_Sprite->SetRGB( 1.5f, 1.5f, 1.5f );
		}

	}

	void Draw(void)
	{
		//Always render the background image first  
		MM_Background->Render_Object(*M_BG);

		Stage_1_1->Render();
		Stage_1_2->Render();
		Stage_1_3->Render();
			
		Stage_2_1->Render();
		Stage_2_2->Render();
		Stage_2_3->Render();

				
		Stage_3_1->Render();
		Stage_3_2->Render();
		
		Stage_3_3->Render();
		Cursor->Render();
		Cursor->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		cursor_particles->Render();

	}

	void Free(void)
	{
		delete M_BG;
		delete MM_Background;
		
		delete Stage_1_1;
		delete Stage_1_2;
		delete Stage_1_3;

		delete Stage_2_1;
		delete Stage_2_2;
		delete Stage_2_3;

		delete Stage_3_1;
		delete Stage_3_2;
		delete Stage_3_3;

		delete Cursor;
		delete Audio;
		
		delete Stage_1_1_Sprite;
		delete Stage_1_2_Sprite;
		delete Stage_1_3_Sprite;
		 
		delete Stage_2_1_Sprite;
		delete Stage_2_2_Sprite;
		delete Stage_2_3_Sprite;
		
		delete Stage_3_1_Sprite;
		delete Stage_3_2_Sprite;
		delete Stage_3_3_Sprite;
		
		delete CURSOR_SPRITE;
	}

	void Unload(void) {}
}