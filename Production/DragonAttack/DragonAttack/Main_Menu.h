/* Start Header ************************************************************************/
/*!
\file       Main_Menu.h
\author     Javon Lee
\par email: xiongweijavon.lee\@digipen.edu
\brief
The Main menu (gamestate) function declarations are done here, as well as the 
libraries and header files necessary to be included.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameStateManager.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collision.h"
#include "Audio_Engine.h"
#include <utility>
#include <iostream>

namespace Main_Menu
{
	void Load(void);

	void Init(void);

	void Update(float dt);

	void Draw(void);

	void Free(void);

	void Unload(void);
}