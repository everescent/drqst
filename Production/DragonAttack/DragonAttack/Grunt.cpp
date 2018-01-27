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
#include <iostream>


Grunt::Grunt(AEGfxVertexList * mesh, const char* texFile)
	: Characters(), Object(mesh, texFile) 
{
}

Grunt::Grunt(AEGfxVertexList * mesh, const char* texFile, float x, float y)
	: Characters(), Object(mesh, texFile)
{
	SetPosition(x, y);
}

Grunt *Create_Grunt()
{
	AEGfxVertexList *mesh = CreateSquare(30.0f);
	const char* tex_Grunt = ".//Textures/download.jpg";

	Grunt *grunt = new Grunt(mesh, tex_Grunt);

	grunt->SetDamage(10);
	

	return  grunt;

}

void Delete_Grunt(Grunt *grunt)
{
	delete grunt;
}