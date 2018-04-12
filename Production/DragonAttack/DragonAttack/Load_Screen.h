/* Start Header ************************************************************************/
/*!
\file       Load_Screen.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for Load screen

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once

namespace Load_Screen
{
  void Load(void);
  void Init(void);
  void Update(float dt);
  void Draw(void);
  void Free(void);
  void Unload(void);
}