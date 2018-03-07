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
#include <utility>
#include <iostream>

namespace Test_Stage1_1
{
	void Init(void);

	void Load(void);

	void Update(float dt);

	void Draw(void);

	void Free(void);

	void Unload(void);
}