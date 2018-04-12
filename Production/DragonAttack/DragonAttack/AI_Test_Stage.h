/* Start Header ************************************************************************/
/*!
\file       AI_Test_Stage.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates ais base on the enum that was passed in

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Particle_System.h"
#pragma once

namespace AI_Test_Stage
{
	void Init(void);

	void Load(void);

	void Update(float dt);

	void Draw(void);

	void Free(void);

	void Unload(void);
}