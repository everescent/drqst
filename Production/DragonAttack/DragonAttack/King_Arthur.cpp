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

#include "King_Arthur.h"


void King_Arthur_Idle();

void King_Arthur_Moving();

void King_Arthur_AvoidingObstace();

void King_Arthur_Attack();

King_Arthur::King_Arthur(Sprite&& t_sprite)
	: Characters(std::move(t_sprite))
{
	/* const float x, y  // to place the position of king arthur
	   scale and translate king arthur's position*/
}
/*
void King_Arthur::Update(float dt)
{

}
*/
void King_Arthur::Pos()
{

}