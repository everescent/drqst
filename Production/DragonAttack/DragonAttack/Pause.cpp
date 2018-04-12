/* Start Header ************************************************************************/
/*!
\file    Pause.cpp
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
Pause class functions are defined here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "Pause.h"
#include <iostream>
#include "Audio_Engine.h"

namespace anon
{
	//Unable to use AE_Engine to Get WinMax as the AE_Engine updates it constantly 

	//These are the max x,y coordinate of a default screen 
	const float FakeWinMax_X = 760.0f;
	const float FakeWinMax_Y = 360.0f;
	const AEVec2 Yes_Pos{};
	const AEVec2 No_Pos{};
	const AEVec2 Confirm_Pos{};
}

//Default initalize the Particle System for Pause Cursor 
Particle_System* Pause::pause_cursor_ps = nullptr;

	Pause::Pause()
	:	M_BG			{ new Transform{} },
		Pause_BG_Sprite	{ new Sprite{ CreateBG(1.0f, 1.0f, "Textures/Black_BG.png")} },
		Displacement	{ 0.0f, 230.0f },
		Cursor_Pos		{ 0.0f, 0.0f}, //just to initalise it 
		Button_W		{50.0f},
		Button_H		{30.0f},
		Cursor_s	{	new Sprite { S_CreateRectangle( 20.0f, 20.0f, "Textures/Bob_Head.png")					}	},
		Resume_s	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Resume_Button_PM.png" )		}	},
		Options_s	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Options_Button_PM.png")		}	},
		Restart_s 	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Restart_Button_PM.png")			}	},
		Quit_s	 	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Quit_Button_PM.png")			}	},
		Quit_MM_s	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/QuitMainMenu_Button_PM.png")		}	},
		Mute_s		{	new Sprite{ S_CreateRectangle(Button_W, Button_H, ".//Textures/Mute_Button_PM.png")		}	},
		FS_s		{	new Sprite{ S_CreateRectangle(Button_W , Button_H, ".//Textures/Fullscreen_Button_PM.png")		}	},
		Howtoplay_s {	new Sprite{ S_CreateRectangle(500.0f , 300.0f, "Textures/HowToPlay.png") } },
		Instructions_s { new Sprite{ S_CreateRectangle(Button_W , Button_H,"Textures/HowToPlay_Button_PM.png") } },
		Yes_s			{ new Sprite{ S_CreateRectangle(Button_W , Button_H, "Textures/Yes.png") } },
		No_s			{ new Sprite{ S_CreateRectangle(Button_W , Button_H, "Textures/No.png") } },
		Confirm_s		{ new Sprite{ S_CreateRectangle(Button_W , Button_H, "Textures/AreYouSure.png") } }, // Consistent with Main Menu
		Cursor			{	new GameObject {	Cursor_s  , Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},
		Resume			{	new GameObject {	Resume_s  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Options			{	new GameObject {	Options_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Restart			{	new GameObject {	Restart_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Quit			{	new GameObject {	Quit_s	  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Quit_MM			{	new GameObject {	Quit_MM_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},
		Mute			{	new GameObject {	Mute_s	  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},
		Fullscreen		{	new GameObject{		FS_s	  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},

		Howtoplay		{	new GameObject {	Howtoplay_s,	Col_Comp()											}	},
		Instructions	{	new GameObject {	Instructions_s,	Col_Comp()											}	},
		Yes				{	new GameObject {	Yes_s,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)						}	},
		No				{	new GameObject {	No_s,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)						}	},
		Confirm			{	new GameObject{		Confirm_s,	Col_Comp()												}	}
		{
				//Initialise Buttons into the vector
			Buttons.push_back(Instructions);
			Buttons.push_back(Resume);
			Buttons.push_back(Options);
			Buttons.push_back(Restart);
			Buttons.push_back(Quit);
			Buttons.push_back(Quit_MM);
			
			//Endsure that all the buttons are active when constructed 
			for (auto& elem : Buttons)
			{
				elem->SetActive(true);
			}
				//Initalise the Options buttons
			Option_Buttons.push_back(Mute);
			Option_Buttons.push_back(Fullscreen);

			Cursor->SetActive(true); //Ensure Cursor is active when constructed 
			
			//Initialize the particle system for the Pause Cursor with the behaviour
			Pause::pause_cursor_ps = Effects_Get(CURSOR_PARTICLE) ; //Get similar coin particles 

			//Redefined Behaviour for cursor particle 
			pause_cursor_ps->Emitter_.PPS_ = 5;
			pause_cursor_ps->Emitter_.Dist_Min_ = 0.0f;
			pause_cursor_ps->Emitter_.Vol_Max = 100;
			pause_cursor_ps->Emitter_.Direction_ = 0.0f; // shoot out to the right 
			pause_cursor_ps->Emitter_.Particle_Rand_.Spread_ = 10;
			pause_cursor_ps->Emitter_.Conserve_ = 0.80f;
			pause_cursor_ps->Emitter_.Size_ = 8.0f;
			pause_cursor_ps->Emitter_.Speed_ = 8.0f;
			pause_cursor_ps->Emitter_.Lifetime_ = 0.17f;
			

		} //end of ctor 

void Pause::Update(bool &pause_bool,const float dt)
{
	
	AEGfxGetCamPosition(&cameraX, &cameraY); //Update_Buttons(cameraX, cameraY);
	AEInputShowCursor(0); //Hide the default cursor at all times 
	// Update the Cursor's Position 

	//2 tmp signed int for using AEInputGetCusorPosition, as that function only takes in signed int and not float.
	s32 tmpX;
	s32 tmpY;
	AEInputGetCursorPosition(&tmpX, &tmpY);
	Cursor_Pos.x = ((float)tmpX - 640.0f) + cameraX + 10.0f;
	Cursor_Pos.y = (360.0f - (float)tmpY) + cameraY - 10.0f;
	 
	Cursor->Transform_.SetTranslate(Cursor_Pos.x, Cursor_Pos.y);
	Cursor->Transform_.Concat();

	//Use a range for to update the collision component for all the PAUSE Buttons 
	for (auto& elem : Buttons)
	{	
		elem->Collision_.Update_Col_Pos(cameraX - Button_W + Displacement.x, cameraY - Button_H + Displacement.y,
										cameraX + Button_W + Displacement.x, cameraY + Button_H + Displacement.y);
		elem->Transform_.SetTranslate(cameraX + Displacement.x, cameraY + Displacement.y);
		elem->Transform_.Concat();
		elem->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//update the Displacement to print the buttons away from each other 
		Displacement.y += -90.0f; 

		//Create a high-light effect when the cursor is over the Button 
		if  (elem->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y) )
		{
			elem->Sprite_->SetRGB(0.7f, 0.7f, 0.7f);
		}
		else
		{
			elem->Sprite_->SetRGB(1.0f, 1.0f, 1.0f);
		}
	}
	//Reset the Displacement
	
	Yes->Collision_.Update_Col_Pos(cameraX - Button_W - 100.0f, cameraY - Button_H - 100.0f,
		cameraX + Button_W -100.0f, cameraY + Button_H -100.0f);

	No->Collision_.Update_Col_Pos(cameraX - Button_W + 100.0f, cameraY - Button_H - 100.0f,
		cameraX + Button_W + 100.0f, cameraY + Button_H - 100.0f);


	if (curr_Screen == Options_Screen)
		{
			
			//Set a different Displacement for Option buttons to centralise it
			Displacement = { 0.0f, 50.0f };

			//Update the button's collision and graphical placement on screen
			for (auto& elem : Option_Buttons) 
			{
			elem->Collision_.Update_Col_Pos(cameraX - Button_W + Displacement.x, cameraY - Button_H + Displacement.y,
				cameraX + Button_W + Displacement.x, cameraY + Button_H + Displacement.y);
			elem->Transform_.SetTranslate(cameraX + Displacement.x, cameraY + Displacement.y);
			elem->Transform_.Concat();
			elem->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

			//update the Displacement to print the buttons away from each other 
			Displacement.y += -100.0f;

			//Create a high-light effect when the cursor is over the Button 
			if (elem->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				elem->Sprite_->SetRGB(0.7f, 0.7f, 0.7f);
			}
			else
			{
				elem->Sprite_->SetRGB(1.0f, 1.0f, 1.0f);
			}

			}
			Displacement = { 0.0f, 50.0f };//Reset

			if (AEInputCheckTriggered(AEVK_LBUTTON))
			{
				if (Fullscreen->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
				{
					fullscreen = fullscreen ? false : true; 
					AEToogleFullScreen(fullscreen);
				}

				if (Mute->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
				{
					Audio_Engine::MUTE_ = Audio_Engine::MUTE_ ? false: true ;
				}
			}

		}
	
	// only do all these checks if the game is paused 
	//AND only if the buttons are displaying 
	else if ( curr_Screen == Pause_Screen )
	{

		// Check if when the Left mouse is clicked, is the Cursor on a Button or not
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			//As the Buttons all have different reactions, it is not unnecessary to use a for range to 
			//Call the Collision Function check. Rather, each button collision with the cursor is checked seperately

			//For the Instructions Button 
			if (Instructions->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				curr_Screen = Howtoplay_Screen;
				Toggle_Button_Display(Howtoplay_Screen);
			}

			//For the Resume Button 
			if (Resume->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				pause_bool = pause_bool == true ? false : true;
			}

			//For the Options Button 
			if (Options->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				Options_screen = Options_screen ? false : true; //toggle the options screen to show/hide
				curr_Screen = Options_Screen;
				//For the Options button, it's reaction would be to call the functions in Options.
				//Set all the buttons to not-Active first 
				Toggle_Button_Display(Options_Screen);
			}

			if (Restart->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				pause_bool = pause_bool == true ? false : true;
				SM::Set_Next(SS_RESTART); // No need for fading effect as the Stage gamestates handle them 
			}

			if (Quit->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				curr_Screen = Confirmation_Screen;
				Toggle_Button_Display(Confirmation_Screen);
			}

			if (Quit_MM->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				menu_click = menu_click ? false : true;
				curr_Screen = Confirmation_Screen;
				Toggle_Button_Display(Confirmation_Screen);
				
				
			}
		}
	}

	else if (curr_Screen == Confirmation_Screen)
	{
		//Translate Concat Confirmation message 
		Confirm->Transform_.SetTranslate(cameraX, cameraY);
		Confirm->Transform_.Concat();
		Confirm_s->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		//T and C Yes and No buttons
		Yes->Transform_.SetTranslate(cameraX - 100.0f, cameraY - 100.0f); // Same Coordinates as main menu 
		Yes->Transform_.Concat();
		Yes_s->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		No->Transform_.SetTranslate(cameraX + 100.0f, cameraY - 100.0f);// Same Coordinates as main menu 
		No->Transform_.Concat();
		No_s->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		if (Yes->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
		{
			Yes_s->SetRGB(0.7f, 0.7f, 0.7f);
		}
		else if (No->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
		{
			No_s->SetRGB(0.7f, 0.7f, 0.7f);
		}
		else
		{
			Yes_s->SetRGB(1.0f, 1.0f, 1.0f);
			No_s->SetRGB(1.0f, 1.0f, 1.0f);
		}

		//If the mouse is clicked 
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			if (Yes->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				if (menu_click)
				{
					pause_bool = pause_bool == true ? false : true;
					menu_click = menu_click ? false : true; //reset variable
					SM::Set_Next(SS_QUIT); // Quit the stage 
					GSM::next = GS_MAIN; // Go back to the Main Menu's gamestate 
				}

				else
					GSM::next = GS_QUIT;
			}

			if (No->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				curr_Screen = Pause_Screen;
				Toggle_Button_Display(Pause_Screen);
			}
		}

	}

	else if (curr_Screen == Howtoplay_Screen)
	{
		Howtoplay->Transform_.SetTranslate(cameraX, cameraY);
		Howtoplay->Transform_.Concat();
		Howtoplay_s->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	}

	//Reset the Displacement
	Displacement = { 0.0f, 230.0f };

	if (AEInputCheckTriggered(AEVK_ESCAPE))
		{
			if (Options_screen) // if i'm at the Option screen, go backy to the pause first 
			{
				Options_screen = Options_screen ? false : true;
				curr_Screen = Pause_Screen;
				Toggle_Button_Display(Pause_Screen); //Toggles between showing the option buttons and pause buttons 
			}
			else if (curr_Screen == Howtoplay_Screen || curr_Screen == Confirmation_Screen)
			{
				curr_Screen = Pause_Screen;
				Toggle_Button_Display(Pause_Screen);
			}
			else 
			pause_bool = pause_bool == true ? false : true;
		}
	
	//Translate and Concat the background according to where the player currently is
	M_BG->SetTranslate(cameraX, cameraY);
	M_BG->Concat();
	Pause_BG_Sprite->SetAlphaTransBM(1.0f, 0.6f, AE_GFX_BM_BLEND);

	//Update the Particle Effects 
	pause_cursor_ps->Emitter_.Pos_.Min_Max.Point_Max.x = Cursor_Pos.x + 480.0f;
	pause_cursor_ps->Emitter_.Pos_.Min_Max.Point_Min.x = Cursor_Pos.x + 10.0f;
	pause_cursor_ps->Emitter_.Pos_.Min_Max.Point_Max.y = Cursor_Pos.y + 10.0f;
	pause_cursor_ps->Emitter_.Pos_.Min_Max.Point_Min.y = Cursor_Pos.y - 10.0f;

	pause_cursor_ps->UpdateEmission();
	pause_cursor_ps->Turbulence(0.7f);
	pause_cursor_ps->TransRamp_Exp();
	pause_cursor_ps->Newton({ Cursor_Pos.x + 20.0f , Cursor_Pos.y - 10.0f }, 0.2f);

	if (pause_cursor_ps->GetParticleCount())
	{
		pause_cursor_ps->Update(dt);
	}
}//End of Pause::Update 

void Pause::Render()
{
	Pause_BG_Sprite->Render_Object(*M_BG);

	//Render the Buttons. 
	//Since the Toggle function will handle the actual rendering or not, it is ok to just call the functions. 
	for (auto& elem : Buttons)
	{
		elem->Render();
	}
	for (auto& elem : Option_Buttons)
	{
		elem->Render();
	}

	//Render Prompt Message 
	Confirm->Render();
	Yes->Render();
	No->Render();;

	//Render How to play instructions
	Howtoplay->Render();

	//Render the cursor particles
	pause_cursor_ps->Render();

	//Render the Mouse Cursor lasts so that it doesn't get hidden by the Buttons 
	Cursor->Render();
	Cursor->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}//End of Pause::Render

Pause::~Pause()
{
	//AEGfxDestroyFont(fontID);
	//Delete the assests related to the pause BG
	delete M_BG;
	delete Pause_BG_Sprite;

	delete Cursor_s;
	delete Resume_s;
	delete Options_s;
	delete Restart_s;
	delete Quit_s;
	delete Quit_MM_s;
	delete Mute_s;
	delete FS_s;

	delete Howtoplay_s;
	delete Instructions_s;
	delete Yes_s;
	delete No_s;
	delete Confirm_s;

	delete Cursor;
	for (auto& elem : Buttons)
	{
		delete elem;
	}

	for (auto& elem : Option_Buttons)
	{
		delete elem;
	}

	delete Howtoplay;
	delete Yes;
	delete No;
	delete Confirm;
}// End of Pause::~Pause


//This function uses the SetActive function provided in GameObject to Render or Not Render the buttons 
void Pause::Toggle_Button_Display(Screen screen)
{
	if (screen == Options_Screen)
	{
		for (auto& elem : Buttons)
		{
			elem->SetActive(false);//Hide Pause butttons 
		}
		for (auto& elem : Option_Buttons)
		{
			elem->SetActive(true); //Show Option buttons 
		}

		Confirm->SetActive(false);
		Yes->SetActive(false);
		No->SetActive(false);
		Howtoplay->SetActive(false);
	}

	else if (screen == Pause_Screen)
	{
		for (auto& elem : Buttons)
		{
			elem->SetActive(true);// Show Pause buttons 
		}
		for (auto& elem : Option_Buttons)
		{
			elem->SetActive(false);//Hide Option buttons 
		}

		Confirm->SetActive(false);
		Yes->SetActive(false);
		No->SetActive(false);
		Howtoplay->SetActive(false);
	}

	else if (screen == Confirmation_Screen)
	{
		Confirm->SetActive(true);
		Yes->SetActive(true);
		No->SetActive(true);
		for (auto& elem : Buttons)
		{
			elem->SetActive(false);//Hide Pause butttons 
		}
		for (auto& elem : Option_Buttons)
		{
			elem->SetActive(false); //Show Option buttons 
		}
		Howtoplay->SetActive(false);
	}

	else 
	{
		for (auto& elem : Buttons)
		{
			elem->SetActive(false);// Show Pause buttons 
		}
		for (auto& elem : Option_Buttons)
		{
			elem->SetActive(false);//Hide Option buttons 
		}

		Confirm->SetActive(false);
		Yes->SetActive(false);
		No->SetActive(false);
		Howtoplay->SetActive(true);
	}


}//End of Pause::Toggle_Button_Display