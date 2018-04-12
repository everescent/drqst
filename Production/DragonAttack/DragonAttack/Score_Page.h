/* Start Header ************************************************************************/
/*!
\file       Score_Page.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for Score_Page

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "Particle_Effects.h" // Effects_Get


void Load_Score_Page(void);
void Init_Score_Page(void);
void Update_Score_Page(const float dt);
void Render_Score_Page(void);
void Free_Score_Page(void);
void Unload_Score_Page(void);
