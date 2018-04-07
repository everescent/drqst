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
#include "GameStateManager.h"
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
	
	
private:
	f32 cameraX, cameraY;
	//u32 fontID;
	//char* buttons[pause_ns::NUM_BUTTONS ] = { "Resume [ESC]",	" Turn Audio On/Off", "Quit [Q]", "Fullscreen [F]" }; // array count from 0
	Transform* M_BG;
	Sprite* Pause_BG_Sprite;
	bool fullscreen = false; // a bool to control whether it's full screen or not 
	AEVec2 	 Displacement; //This represents the displacement between the various buttons 
	AEVec2 Cursor_Pos;
	//Set a constant width and height for the buttons
	const float Button_W;
	const float Button_H;
	Sprite* Cursor_s;
	//using Sprites to implement the mesh/drawings of the buttons 
	Sprite* Resume_s  ;
	Sprite*	Options_s ;
	Sprite*	Restart_s ;
	Sprite*	Quit_s	  ;
	Sprite*	Quit_MM_s ;
	//Create a GameObject to instantiate the custom cursor
	GameObject* Cursor;
	//using GameObjects to instantiate Different buttons 
	GameObject* Resume	;
	GameObject* Options	;
	GameObject* Restart	;
	GameObject* Quit	;
	GameObject* Quit_MM	;

	std::vector<GameObject*> Buttons;

};