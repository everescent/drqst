/* Start Header ************************************************************************/
/*!
\file       Characters.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Character class, parent class of the characters to be created

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include <utility>
#include "GameObject.h"
#include "Transform.h"
#include "Create_Object.h"
#include "Animation.h"
#include "AI_Attack_Sprites.h"

enum Direction
{
	LEFT = 0,
	RIGHT,
	TOP,
	DOWN,
	STAY
};

class Dragon; //forward declaration

// A class that creates a basic character
class Characters : public GameObject
{
	private:
		int hp;                    // hp the character have
		Direction direction;       // moving in left or right, up or down?
		static int score;          // player score for the stage
		static int enemies_killed; // number of enemies killed
		float idle_timing;         // idle timing for mob
		bool vulnerable;  	       // immune to damage if false

	public:
		Characters(Sprite* t_sprite, const int HP, Col_Comp &&t_col); // default constructor
    virtual ~Characters() = default;
		int Get_HP() const;         // get HP of mob
		void Set_HP(int hp);       // set the hp for mob
		void Increase_HP(const int increment);      // increase hp via power ups / status
		void Decrease_HP(const char damage = 1);    // decrease the hp of the character

		Direction Get_Direction() const; // get the direction of mob
		void Set_Direction(Direction direction);// set the direction of mob

		float Get_Idle_Time() const;
		void  Reset_Idle_Time(float);
		void  Decrease_Idle_Time(float);

		void Set_Vulnerable(const bool status);
		bool Get_Vulnerable(void) const;

		//------------------------------------------------------------------------------------------
		//STATIC FUNCTIONS--------------------------------------------------------------------------
		static int Get_Score();                 // get current score
		static void Add_Score(const int score); // add score
		static void Reset_Score();              // reset score

		static int Get_Enemies_Killed();        // get enemies killed
		static void Add_Kill_count();			// add enemy kill count
		static void Reset_Enemy_Killed();		// reset enemy kill count

    virtual void Update(Dragon& d, const float dt) = 0/*{ UNREFERENCED_PARAMETER(d); UNREFERENCED_PARAMETER(dt); }*/;
};
