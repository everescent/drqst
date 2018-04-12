/* Start Header ************************************************************************/
/*!
\file    Pause.cpp
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
Pause class is declared here, together with it's members and functions 

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
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
#include "Options.h"
#include "Particle_System.h"
#include "Particle_Effects.h"

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
	
	//This function checks if the pause button is triggered, if it is it changes the boolian to true
	void Update(bool &pause_bool, const float dt);

	void Render();//This function renders the pause screen and all related objects 
	
	~Pause();//A non-default destructor to prevent memory leak 
	enum Screen
	{
		Pause_Screen,
		Confirmation_Screen,
		Options_Screen,
		Howtoplay_Screen
	};
	
private:
	f32 cameraX, cameraY; //float variables to store the camera position
	//u32 fontID;
	//char* buttons[pause_ns::NUM_BUTTONS ] = { "Resume [ESC]",	" Turn Audio On/Off", "Quit [Q]", "Fullscreen [F]" }; // array count from 0
	Transform* M_BG; // Transform class to store the transformation matrix for the Background 
	Sprite* Pause_BG_Sprite; //A black background sprite to create a 'shaded' effect 
	bool Options_screen = false; // a bool to check if it's on the pause screen or not 
	AEVec2 	 Displacement; //This represents the displacement between the various buttons 
	AEVec2 Cursor_Pos;
	//Set a constant width and height for the buttons
	const float Button_W;
	const float Button_H;
	Sprite* Cursor_s; // sprite for cursor 

	//using Sprites to implement the mesh/drawings of the buttons 
	Sprite* Resume_s  ;
	Sprite*	Options_s ;
	Sprite*	Restart_s ;
	Sprite*	Quit_s	  ;

	Sprite*	Quit_MM_s ;// Quit button sprite to Main Menu 
	//For the Options 
	Sprite* Mute_s;
	Sprite* FS_s; 
	Sprite* Howtoplay_s;
	Sprite* Instructions_s;
	Sprite* Yes_s;
	Sprite* No_s;
	Sprite* Confirm_s;
	//Create a GameObject to instantiate the custom cursor
	GameObject* Cursor;
	//using GameObjects to instantiate Pause buttons 
	GameObject* Resume	;
	GameObject* Options	;
	GameObject* Restart	;
	GameObject* Quit	;
	GameObject* Quit_MM	;
	//using GameObjects to instantiate the Option buttons 
	GameObject* Mute		;
	GameObject* Fullscreen;
	bool fullscreen = true; 
	bool muting = false;
	Screen curr_Screen;
	//Particle system for cursor
	static Particle_System* pause_cursor_ps;

	GameObject* Howtoplay;
	GameObject* Instructions;
	GameObject* Yes;
	GameObject* No;
	GameObject* Confirm;



	std::vector<GameObject*> Buttons;
	std::vector <GameObject*> Option_Buttons;

	void Toggle_Button_Display(Screen screen); //This function toggles the display of all the buttons. 

};

