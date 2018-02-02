/* Start Header ************************************************************************/
/*!
\file       Grunt.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Grunts to be created for the game.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Characters.h"

class Grunt : public  Characters
{
private: 

public:
	/******************************************************************************/
	/*!
	\fn         Grunt

	\brief      Constructor for a Grunt Object

	\param      Sprite&& t_sprite
					- an rvalue of the sprite to be moved

	\retval     void
	No return.
	*/
	/******************************************************************************/
	Grunt(Sprite&& t_sprite);

	/******************************************************************************/
	/*!
	\fn         Grunt

	\brief      Constructor for a Grunt Object

	\param      Sprite&& t_sprite
					- an rvalue of the sprite to be moved
				x 
					- location to spawn the enemy
				y 
					- location to spawn the enemy

	\retval     void
	No return.
	*/
	/******************************************************************************/
	//Grunt(Sprite&& t_sprite, const float x, const float y);

<<<<<<< HEAD
	void Update(); //ignored
	void Update(float dt);
	void Pos();
=======
	void Update();
	void Pos(Grunt *grunt);
>>>>>>> c21747bc9a97357ff2abd4a67bc3f8305d6b05f1

};

/******************************************************************************/
/*!
\fn         void *Create_Grunt

\brief      Creates a grunt enemy

\param      void

\retval     void
No return.
*/
/******************************************************************************/
Grunt *Create_Grunt();

/******************************************************************************/
/*!
\fn         void *Delete_Grunt

\brief      Deletes a grunt enemy

\param      A pointer to a grunt object

\retval     void
No return.
*/
/******************************************************************************/
void Delete_Grunt(Grunt* grunt);

void MoveTowardPlayer();

void AttackPlayer();

void Idle();