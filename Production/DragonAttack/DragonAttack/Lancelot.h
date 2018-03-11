/* Start Header ************************************************************************/
/*!
\file       Lancelot.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the first boss Lancelot

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Characters.h"
#include "Dragon.h"
#include "Boss_Phase.h"

class Lancelot : public Characters
{
private:
	void Idle(const Dragon &  , const float dt); // lancelot idle state
	void Moving(const Dragon& , const float dt); // moving state of lancelot
	void Attack(Dragon &      , const float dt); // attack state of lancelot
	void Dead(void);

	void Init(void); //initializer for lancelot
	void Init_Stab(void);
	void Init_Slash(void);
	void Init_Arondight(void);
	void Set_Face_Dir(const Dragon &);
	void Set_Attk_Dir(void); // set the attack direction of lancelot

	void Lancelot_Phase2(const float dt);

	void Stab(Dragon&, const float dt); // first basic attack of lancelot
	void Slash(Dragon&, const float dt); // second basic attack of lancelot
	void Mad_Enhancement(const float dt); // makes lancelot move faster, shorter cooldown
	void Arondight(Dragon&, const float dt); // phase 2 mechanism, long range attack

	BOSS_PHASE phase = PHASE_1;  // flag for lancelot second phase

	bool M_E; // lancelot berserk state

public:
	Lancelot(const Sprite*);  // default constructor for lancelot
	~Lancelot(); // destructor for lancelot
	void Update(Dragon &player, const float dt);
	void Render(void);

};