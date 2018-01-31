/* Start Header ************************************************************************/
/*!
\file       King_Arthur.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the last boss king arthur

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Characters.h"

class King_Arthur : public Characters
{
private:
	void Idle(float dt);

	void Moving();

	void AvoidingObstacle();

	void Attack();

	bool phase2;

public:
	/******************************************************************************/
	/*!
	\fn         King Arthuer

	\brief      Constructor for King Arthur

	\param      Sprite&& t_sprite
	- an rvalue of the sprite to be moved

	\retval     void
	No return.
	*/
	/******************************************************************************/
	King_Arthur(Sprite&& t_sprite);

	void Update(float dt);
	void Update() {};
	void Pos();
};



