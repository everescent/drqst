/* Start Header ************************************************************************/
/*!
\file    Level_Selector.h
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
This is a game state that allows the player to select different levels to go too.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
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
#include "StageManager.h"
#include <utility>
#include <iostream>

namespace Level_Selector
{
	void Load(void);

	void Init(void);

	void Update(float dt);

	void Draw(void);

	void Free(void);

	void Unload(void);
}