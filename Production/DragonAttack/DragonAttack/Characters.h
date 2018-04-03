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
#include "Particle_Effects.h"

/**************************************************************************************
//
// enumeration for the direction the ai should face
//
**************************************************************************************/
enum Direction
{
	LEFT = 0,
	RIGHT,
	TOP,
	DOWN,
	STAY
};

/**************************************************************************************
//
// enumeration for the basic character animations
//
**************************************************************************************/
enum
{
	HIT_ANIM,
	IDLE_ANIM,
	WALK_ANIM,
};

class Dragon; //forward declaration

/**************************************************************************************
//
// An abstract class that stores variables which will be needed by AIs
//
**************************************************************************************/
class Characters : public GameObject
{
	private:
		//------------------------------------------------------------------
		//
		//  CHARACTER VARIABLES
		//
		//------------------------------------------------------------------
		int              hp;                   // hp the character have
		Direction        direction;            // moving in left or right, up or down?
		static int       score;                // player score for the stage
		static int       enemies_killed;       // number of enemies killed
		float            idle_timing;          // idle timing for mob
		bool             vulnerable;  	       // immune to damage if false

	public:
		//------------------------------------------------------------------
		//
		//  FUNCTIONS FOR CHARACTERS
		//
		//------------------------------------------------------------------
		Characters(Sprite* t_sprite, const int HP, Col_Comp &&t_col); // default constructor
        virtual ~Characters() = default;                              // virtual destructor
		
	    //------------------------------------------------------------------
	    //
	    //  FUNCTIONS THAT AFFECTS THE HP OF CHARACTERS
	    //
	    //------------------------------------------------------------------
		int Get_HP() const;                        // get HP of mob
		void Set_HP(int hp);                       // set the hp for mob
		void Increase_HP(const int increment);     // increase hp via power ups / status
		void Decrease_HP(const char damage = 1);   // decrease the hp of the character

	    //------------------------------------------------------------------
	    //
	    //  SETTERS AND GETTERS FOR DIRECTION
	    //
	    //------------------------------------------------------------------
	    Direction Get_Direction() const;               // get the direction of mob
		void      Set_Direction(Direction direction);  // set the direction of mob

	 	//------------------------------------------------------------------
	    //
	    //  FUNCTIONS THAT AFFECTS THE IDLE DURATION
	    //
	    //------------------------------------------------------------------
		float Get_Idle_Time() const;
		void  Reset_Idle_Time(float);
		void  Decrease_Idle_Time(float);

		//------------------------------------------------------------------
		//
		//  GETTERS AND SETTERS FOR VULNERABILITY OF CHARACTERS
		//
		//------------------------------------------------------------------
		void Set_Vulnerable(const bool status);
		bool Get_Vulnerable(void) const;

		//------------------------------------------------------------------
		//
		//  STATIC VARIABLES AND FUNCTION
		//
		//------------------------------------------------------------------
		static int  Get_Score();                 // get current score
		static void Add_Score(const int score);  // add score
		static void Reset_Score();               // reset score

		static int  Get_Enemies_Killed();        // get enemies killed
		static void Add_Kill_count();			 // add enemy kill count
		static void Reset_Enemy_Killed();		 // reset enemy kill count

		// virtual destructor of characters
        virtual void Update(Dragon& d, const float dt) = 0;
};
