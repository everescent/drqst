/* Start Header ************************************************************************/
/*!
\file       Pause.cpp
\author     Javon Lee Xiong Wei
\par email: xiongweijavon.lee\@digipen.edu
\brief
Pause class body declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Pause.h"
#include <iostream>

namespace
{
	const float FakeWinMax_X = 760.0f;
	const float FakeWinMax_Y = 360.0f;
}

	Pause::Pause()
	:	M_BG			{ new Transform{} },
		Pause_BG_Sprite	{ new Sprite{ CreateBG(1.0f, 1.0f, "Textures/Black_BG.png")} },
		Displacement	{ 0.0f, 230.0f },
		Cursor_Pos		{ 0.0f, 0.0f}, //just to initalise it 
		Button_W		{50.0f},
		Button_H		{30.0f},
		Cursor_s	{	new Sprite { S_CreateRectangle( 20.0f, 20.0f, "Textures/Bob_Head.png")					}	},
		Resume_s	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Resume_Button.png" )		}	},
		Options_s	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Options_Button.png")		}	},
		Restart_s 	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Play_Button.png")			}	},
		Quit_s	 	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Quit_Button.png")			}	},
		Quit_MM_s	{	new Sprite { S_CreateRectangle (80.0f, 60.0f, "Textures/QuitMainMenu_Button.png")		}	},
		Mute_s		{	new Sprite{ S_CreateRectangle(Button_W, Button_H, ".//Textures/Mute_Button.png")		}	},
		FS_s		{	new Sprite{ S_CreateRectangle( 80.0f , 45.0f, ".//Textures/Fullscreen_Button.png")		}	},
		Cursor		{	new GameObject {	Cursor_s  , Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},
		Resume		{	new GameObject {	Resume_s  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Options		{	new GameObject {	Options_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Restart		{	new GameObject {	Restart_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Quit		{	new GameObject {	Quit_s	  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Quit_MM		{	new GameObject {	Quit_MM_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},
		Mute		{	new GameObject {	Mute_s	  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},
		Fullscreen	{	new GameObject{		FS_s	  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	}

		{
				//Initialise Buttons 
			Buttons.push_back(Resume);
			Buttons.push_back(Options);
			Buttons.push_back(Restart);
			Buttons.push_back(Quit);
			Buttons.push_back(Quit_MM);
			for (auto& elem : Buttons)
			{
				elem->SetActive(true);
			}
				//Initalise the Options buttons
			Option_Buttons.push_back(Mute);
			Option_Buttons.push_back(Fullscreen);

			Cursor->SetActive(true);

			//Initialise the Option buttons/objects 
			Init_Options();

		} //end of ctor 

void Pause::Update(bool &pause_bool)
{
	AEGfxGetCamPosition(&cameraX, &cameraY);
	//Update_Buttons(cameraX, cameraY);
	/*what to update if the game is paused*/
	
	AEInputShowCursor(0); //Hide the default cursor at all times 
	// Update the Cursor's Position 

	s32 tmpX;
	s32 tmpY;
	AEInputGetCursorPosition(&tmpX, &tmpY);
	Cursor_Pos.x = ((float)tmpX - FakeWinMax_X) + cameraX + 140.0f;
	Cursor_Pos.y = (FakeWinMax_Y - (float)tmpY) + cameraY -10.0f;
	 
	Cursor->Transform_.SetTranslate(Cursor_Pos.x, Cursor_Pos.y);
	Cursor->Transform_.Concat();

	//Use a range for to update the collision component for all the Buttons 
	for (auto& elem : Buttons)
	{	
		elem->Collision_.Update_Col_Pos(cameraX - Button_W + Displacement.x, cameraY - Button_H + Displacement.y,
										cameraX + Button_W + Displacement.x, cameraY + Button_H + Displacement.y);
		elem->Transform_.SetTranslate(cameraX + Displacement.x, cameraY + Displacement.y);
		elem->Transform_.Concat();
		elem->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		//update the Displacement to print the buttons away from each other 
		Displacement.y += -90.0f; 
	}
	//Reset the Displacement
	

	if (Options_screen)
		{
			//Set a different Displacement for Option buttons to centralise it
			Displacement = { 0.0f, 50.0f };
			for (auto& elem : Option_Buttons)
			{
			elem->Collision_.Update_Col_Pos(cameraX - Button_W + Displacement.x, cameraY - Button_H + Displacement.y,
				cameraX + Button_W + Displacement.x, cameraY + Button_H + Displacement.y);
			elem->Transform_.SetTranslate(cameraX + Displacement.x, cameraY + Displacement.y);
			elem->Transform_.Concat();
			elem->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			//update the Displacement to print the buttons away from each other 
			Displacement.y += -100.0f;
			}
			Displacement = { 0.0f, 50.0f };//Reset

			if (AEInputCheckTriggered(AEVK_LBUTTON))
			{
				if (Fullscreen->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
				{
					pause_bool = pause_bool == true ? false : true;
				}
			}

		}
	//Reset the Displacement
	Displacement = { 0.0f, 230.0f };


	// only do all these checks if the game is paused 
	//AND only if the buttons are displaying 
	if (pause_bool && !Options_screen)
	{

		// Check if when the Left mouse is clicked, is the Cursor on a Button or not
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			//As the Buttons all have different reactions, it is not unnecessary to use a for range to 
			//Call the Collision Function check. Rather, each button collision with the cursor is checked seperately

			//For the Resume Button 
			if (Resume->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				pause_bool = pause_bool == true ? false : true;
			}

			//For the Options Button 
			if (Options->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				Options_screen = Options_screen ? false : true; //toggle the options screen to show/hide
				//For the Options button, it's reaction would be to call the functions in Options. 
				std::cout << "Is options on?" << Options_screen << std::endl;
				//Set all the buttons to not-Active first 
				Toggle_Button_Display();
			}

			if (Restart->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				pause_bool = pause_bool == true ? false : true;
				SM::Set_Next(SS_RESTART);
			}

			if (Quit->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				GSM::next = GS_QUIT;
			}

			if (Quit_MM->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				pause_bool = pause_bool == true ? false : true;
				
				GSM::next = GS_MAIN;

			}
		}

	}

		if (AEInputCheckTriggered(AEVK_ESCAPE))
		{
			if (Options_screen) // if i'm at the Option screen, go backy to the pause first 
			{
				Options_screen = Options_screen ? false : true;
				Toggle_Button_Display();
			}
			else 
			pause_bool = pause_bool == true ? false : true;
		}
	
	//cameraX -= 100.0f;
	M_BG->SetTranslate(cameraX, cameraY);
	M_BG->Concat();
	Pause_BG_Sprite->SetAlphaTransBM(1.0f, 0.6f, AE_GFX_BM_BLEND);
}

void Pause::Render()
{
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	
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

	//Render the Mouse Cursor lasts so that it doesn't get hidden by the Buttons 
	Cursor->Render();
	Cursor->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	/////////////////// UNUSED CODE AS CHANGED IMPLEMENTATION TO BUTTONS ///////////////////////
	//Set the Render mode for rendering fonts 
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR); // render with color
	//AEGfxTextureSet(NULL, 0, 0);		 // no texture needed
	//AEGfxSetTransparency(1.0f);

	//AEGfxPrint(fontID, buttons[0], (s32)cameraX - 100, (s32)cameraY +	 100	, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, buttons[1], (s32)cameraX - 100, (s32)cameraY			, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, buttons[2], (s32)cameraX - 100, (s32)cameraY -  100 , 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, buttons[3], (s32)cameraX - 100, (s32)cameraY -	 200	, 1.0f, 1.0f, 1.0f);
	//std::cout << "Pause Render called" << std::endl;
	/////////////////// UNUSED CODE AS CHANGED IMPLEMENTATION TO BUTTONS ///////////////////////
}

Pause::~Pause()
{
	//AEGfxDestroyFont(fontID);
	delete Pause_BG_Sprite;
	delete M_BG;
	delete Cursor;
	for (auto& elem : Buttons)
	{
		delete elem;
	}
	for (auto& elem : Option_Buttons)
	{
		delete elem;
	}
	delete Cursor_s;
	delete Resume_s;
	delete Options_s;
	delete Restart_s;
	delete Quit_s;
	delete Quit_MM_s;
	delete Mute_s;
	delete FS_s;
	
}

void Pause::Toggle_Button_Display()
{
	if (Options_screen)
	{
		for (auto& elem : Buttons)
		{
			elem->SetActive(false);
		}
		for (auto& elem : Option_Buttons)
		{
			elem->SetActive(true);
		}
	}

	else
	{
		for (auto& elem : Buttons)
		{
			elem->SetActive(true);
		}
		for (auto& elem : Option_Buttons)
		{
			elem->SetActive(false);
		}
	}
}