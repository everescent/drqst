/* Start Header ************************************************************************/
/*!
\file	 Stage3_2.h
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
Declarations for the stage 3-2 game state.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "Collision.h"
#include "GameStateManager.h"
#include "Dragon.h"
#include "Floor.h"
#include "Wall.h"
#include "Platform.h"
#include "Barrier.h"
#include "Tower.h"
#include "Scarecrow.h"
#include "Audio_Engine.h"
#include "AI_Data_Factory.h"
#include "Level_Import.h"
#include "LevelChangePlatform.h"
#include "UI.h"
#include "Pause.h"
#include <utility>
#include <iostream>

namespace Stage3_2
{
	void Init(void);
	void Load(void);
	void Update(float dt);
	void Draw(void);
	void Free(void);
	void Unload(void);
}