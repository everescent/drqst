/* Start Header ************************************************************************/
/*!
\file    UI.h
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
UI class is declared here, together with it's members and functions

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
#include "Particle_Effects.h"
#include "GameStateManager.h"
#include "StageManager.h"
/****************************************************************************************
Description:
	This is a class that implements the User Interface. It controls the printing of
	the hp icons and tells the user if the mega-fireball is charged up
****************************************************************************************/
class UI
{
public:
/****************************************************************************************
Description:
Default constructor for User interface that takes in a ptr to Dragon as a parameter
****************************************************************************************/
 
	UI(Dragon* dragon);

/****************************************************************************************
Description:
Default destructor for UI
****************************************************************************************/
	~UI();

/****************************************************************************************
Description:
Updates the UI
****************************************************************************************/
	void UI_Update(Dragon* dragon, const float dt);
/****************************************************************************************
Description:
Renders the UI
****************************************************************************************/
	void Render();
private:

	Sprite*  HP_Sprite;
	Sprite* Charge_Sprite;
	const float icon_w;
	Transform* HP_Trans;
	GameObject* charge_icon;
	int Dragon_hp;
	int Fireball_charge;
	static Particle_System* flame_particles;
	AEVec2 CamPos{ 0.0f , 0.0f }; 
	Sprite black;
	Transform black_trans;
	static float visual_effect;
	static float timer_UI;

};