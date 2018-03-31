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

Pause::Pause()
	
{
	fontID = AEGfxCreateFont("calibri", 32, true, false);
	//M_BG = new Transform{};
	//Pause_BG_Sprite = new Sprite{ CreateBG(1.0f, 1.0f, nullptr,1.0f,1.0f, 0xFF000000) };
	 
}

void Pause::Update(bool &pause_bool)
{
	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		pause_bool = pause_bool ==true?false:true;
	}
	AEGfxGetCamPosition(&cameraX, &cameraY);
	cameraX -= 100.0f;

}

void Pause::Render()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//Set the Render mode for rendering fonts 
	AEGfxSetRenderMode(AE_GFX_RM_COLOR); // render with color
	AEGfxTextureSet(NULL, 0, 0);		 // no texture needed
	AEGfxSetTransparency(1.0f);

	AEGfxPrint(fontID, buttons[0], (s32)cameraX, (s32)cameraY +	 100	, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, buttons[1], (s32)cameraX, (s32)cameraY			, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, buttons[2], (s32)cameraX, (s32)cameraY -  100 , 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, buttons[3], (s32)cameraX, (s32)cameraY -	 200	, 1.0f, 1.0f, 1.0f);
	std::cout << "Pause Render called" << std::endl;
}

Pause::~Pause()
{
	AEGfxDestroyFont(fontID);
}