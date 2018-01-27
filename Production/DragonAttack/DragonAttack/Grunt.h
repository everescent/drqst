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

class Grunt : public virtual Characters
{


public:
	/******************************************************************************/
	/*!
	\fn         Grunt

	\brief      Constructor for a Grunt Object

	\param      mesh    - size of character?
				texFile - texture for character 

	\retval     void
	No return.
	*/
	/******************************************************************************/
	Grunt(AEGfxVertexList * mesh, const char* texFile);

	/******************************************************************************/
	/*!
	\fn         Grunt

	\brief      Constructor for a Grunt Object

	\param      mesh    - size of character?
				texFile - texture for character
				x       - location to spawn the enemy
				y       - location to spawn the enemy

	\retval     void
	No return.
	*/
	/******************************************************************************/
	Grunt(AEGfxVertexList * mesh, const char* texFile, float x, float y);

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

