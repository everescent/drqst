/* Start Header ************************************************************************/
/*!
\file       Stage3_3.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for Stage3_3

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once

#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameStateManager.h"
#include "Dragon.h"
#include "Floor.h"
#include "Wall.h"
#include "Platform.h"
#include "Barrier.h"
#include "Audio_Engine.h"
#include "AI_Data_Factory.h"
#include "Level_Import.h"
#include "LevelChangePlatform.h"
#include "UI.h"
#include "Boss_Phase.h"
#include "Pause.h"
#include <utility>
#include <iostream>

namespace Stage3_3
{
	void Load(void);
	void Init(void);
	void Update(float dt);
	void Draw(void);
	void Free(void);
	void Unload(void);
}