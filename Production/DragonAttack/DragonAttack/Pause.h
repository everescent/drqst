/* Start Header ************************************************************************/
/*!
\file       Pause.h
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
#include "Dragon.h"
#include "Collision.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "AEEngine.h"
#include "Transform.h"
#include "PickUp.h"
#include "Collision.h"
#define PAUSE_MACRO

#ifdef PAUSE_MACRO
namespace pause_ns
{
	const unsigned NUM_BUTTONS = 4; 
}
#endif



class Pause
{
public:
	Pause();
	//pause
	//This function checks if the pause button is triggered, if it is it changes the boolian to true
	void Update(bool &pause_bool);
	void Render();
	////Renders the UI
	//void Pause_Render();
	~Pause();
	//Used to store the position of the camera 
	f32 cameraX, cameraY;
	Col_Comp Audio_box, Quit_box, FS_box;
private:
	u32 fontID;
	char* buttons[pause_ns::NUM_BUTTONS ] = { "Resume [ESC]",	" Turn Audio On/Off", "Quit [Q]", "Fullscreen [F]" }; // array count from 0
	Transform* M_BG;
	Sprite* Pause_BG_Sprite;
	s32 ShowCursor = 1;
	s32 HideCursor = 0;
	bool fullscreen = false;

};