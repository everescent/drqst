/* Start Header ************************************************************************/
/*!
\file       Pause.cpp
\author     Javon Lee Xiong Wei
\par email: xiongweijavon.lee\@digipen.edu
\brief
UI class body declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Pause.h"
#include <iostream>

Pause::Pause()
	
{
	fontID			= AEGfxCreateFont("calibri", 32, true, false);
	M_BG			= new Transform{};
	Pause_BG_Sprite = new Sprite{ CreateBG(1.0f, 1.0f, "Textures/Black_BG.png")};
	//*********Construct the 'buttons' for the Pause Interface *********************************
	/*Audio_box	{	};
	Quit_box	{	};
	FS_box		{	};*/
	
}

//void Pause::Update_Buttons(f32 cameraX, f32 cameraY)
//{
//	Audio_pos.x += cameraX;
//	Audio_pos.y += cameraY;
//	Quit_pos.x += cameraX;
//	Quit_pos.y += cameraY;
//	FS_pos.x += cameraX;
//	FS_pos.y += cameraY;
//}

void Pause::Update(bool &pause_bool)
{
	
	AEGfxGetCamPosition(&cameraX, &cameraY);
	//Update_Buttons(cameraX, cameraY);
	/*what to update if the game is paused*/
	if (pause_bool) 
	{
		AEInputShowCursor(ShowCursor);
		if (AEInputCheckTriggered(AEVK_F))
		{
			fullscreen = fullscreen == true ? false : true;	 
			AEToogleFullScreen(fullscreen);
			
		}
	}
	else { AEInputShowCursor(HideCursor); }
	
	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		pause_bool = pause_bool ==true?false:true;
	}
	
	if (AEInputCheckTriggered(AEVK_Q))
	{
		GSM::next = GS_QUIT;
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
	//Set the Render mode for rendering fonts 
	AEGfxSetRenderMode(AE_GFX_RM_COLOR); // render with color
	AEGfxTextureSet(NULL, 0, 0);		 // no texture needed
	AEGfxSetTransparency(1.0f);

	AEGfxPrint(fontID, buttons[0], (s32)cameraX - 100, (s32)cameraY +	 100	, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, buttons[1], (s32)cameraX - 100, (s32)cameraY			, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, buttons[2], (s32)cameraX - 100, (s32)cameraY -  100 , 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, buttons[3], (s32)cameraX - 100, (s32)cameraY -	 200	, 1.0f, 1.0f, 1.0f);
	//std::cout << "Pause Render called" << std::endl;
}

Pause::~Pause()
{
	AEGfxDestroyFont(fontID);
	delete Pause_BG_Sprite;
	delete M_BG;
}