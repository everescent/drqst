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
		Displacement	{ 0.0f, 330.0f },
		Cursor_Pos		{ 0.0f, 0.0f}, //just to initalise it 
		Button_W		{50.0f},
		Button_H		{30.0f},
		Cursor_s	{	new Sprite { S_CreateRectangle( 20.0f, 20.0f, "Textures/Bob_Head.png")					}	},
		Resume_s	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Resume_Button.png" )		}	},
		Options_s	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Options_Button.png")		}	},
		Restart_s 	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Play_Button.png")			}	},
		Quit_s	 	{	new Sprite { S_CreateRectangle (Button_W, Button_H, "Textures/Quit_Button.png")			}	},
		Quit_MM_s	{	new Sprite { S_CreateRectangle (80.0f, 60.0f, "Textures/QuitMainMenu_Button.png")	}	},
		Cursor		{	new GameObject {	Cursor_s  , Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},
		Resume		{	new GameObject {	Resume_s  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Options		{	new GameObject {	Options_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Restart		{	new GameObject {	Restart_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Quit		{	new GameObject {	Quit_s	  ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	},			
		Quit_MM		{	new GameObject {	Quit_MM_s ,	Col_Comp(0.0f, 0.0f, 0.0f, 0.0f, Rect)					}	}			
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

			Cursor->SetActive(true);

		} //end of ctor 

void Pause::Update(bool &pause_bool)
{
	
	AEGfxGetCamPosition(&cameraX, &cameraY);
	//Update_Buttons(cameraX, cameraY);
	/*what to update if the game is paused*/
	
	AEInputShowCursor(1); //Hide the default cursor at all times 
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
	Displacement = { 0.0f, 100.0f };

	if (pause_bool)
	{
		if (AEInputCheckTriggered(AEVK_F))
		{
			fullscreen = fullscreen == true ? false : true;
			AEToogleFullScreen(fullscreen);

		}

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			if (Resume->Collision_.St_Rect_Point(Cursor_Pos.x, Cursor_Pos.y))
			{
				pause_bool = pause_bool == true ? false : true;
			}
		}

		if (AEInputCheckTriggered(AEVK_Q))
		{
			GSM::next = GS_QUIT;
		}
	}

		

		if (AEInputCheckTriggered(AEVK_ESCAPE))
		{
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
	for (auto& elem : Buttons)
	{
		elem->Render();
	}

	//Render the Mouse Cursor lasts so that it doesn't get hidden by the Buttons 
	Cursor->Render();
	Cursor->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	//Set the Render mode for rendering fonts 

	/////////////////// UNUSED CODE AS CHANGED IMPLEMENTATION TO BUTTONS ///////////////////////
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
}

