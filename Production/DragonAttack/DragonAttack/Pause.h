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
class Pause
{
public:
	//Default constructor for User interface that takes in a ptr to Dragon as a parameter 
	Pause();
	//Updates the UI
	void Pause_update(Dragon* dragon);
	//Renders the UI
	void Pause_Render();

private:

	Sprite  HP_Sprite;
	Sprite Charge_Sprite;
	const float icon_w;
	GameObject hp_icon1;
	GameObject hp_icon2;
	GameObject hp_icon3;
	GameObject charge_icon;
	int Dragon_hp;
	int Fireball_charge;
	//AEVec2 offset1;
};