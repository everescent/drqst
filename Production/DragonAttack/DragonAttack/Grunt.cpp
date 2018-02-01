/* Start Header ************************************************************************/
/*!
\file       Grunt.cpp
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
#include "Grunt.h"

namespace // global variables just for THIS file
{
	const int grunt_hp = 30;

	Col_Comp t_col{ 0.0f, 0.0f, 5.0f, 5.0f, Rect };   // set the shape for grunt here
}


Grunt::Grunt(Sprite&& t_sprite)
	: Characters(std::move(t_sprite), grunt_hp, t_col)
{
	//float scaleX, scaleY;

	//(void)SetScale(scaleX, scaleY);
}

//Grunt::Grunt(Sprite&& t_sprite, const float x, const float y)
//	: Characters(std::move(t_sprite))
//{
//	//float scaleX, scaleY;
//
//	//(void)SetScale(scaleX, scaleY);
//	//translate the position of the grunt as well.
//}




Grunt *Create_Grunt()
{
	const char* tex_Grunt = ".//Textures/download.jpg";

	Grunt *grunt = new Grunt{ S_CreateSquare(100.0f, 1.0f, 1.0f, tex_Grunt) };
	

	return  grunt;

}

void Delete_Grunt(Grunt *grunt)
{
	delete grunt;
}

void Grunt::Update()
{
	//update the behaviour of the grunt
}

void Grunt::Pos()
{

}
