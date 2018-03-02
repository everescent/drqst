/* Start Header ************************************************************************/
/*!
\file       Grunt.h
\author     Andrew Chong Jiahao
\par email: c.jiahaoandrew\@digipen.edu
\brief
Grunts to be created for the game.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Characters.h"
#include "Dragon.h"

class Grunt : public  Characters
{
private:
	void LineOfSight(const Dragon &d);

	void MoveTowardPlayer(const Dragon &d);

	void AttackPlayer(const Dragon &d);

	void Idle(const Dragon &d);
public:
	/******************************************************************************/
	/*!
	\fn         Grunt

	\brief      Constructor for a Grunt Object

	\param      float x, float y
	- x and y coordinates of the object

	\retval     void
	No return.
	*/
	/******************************************************************************/
	Grunt(float x, float y);

	/******************************************************************************/
	/*!
	\fn         Update

	\brief      Update function for a Grunt Object

	\param      Dragon &d, const float dt
	d
	- reference to player
	dt
	- delta time

	\retval     void
	No return.
	*/
	/******************************************************************************/
	void Update(Dragon &d, const float dt);

	//~Grunt(); // destructor
};
