/* Start Header ************************************************************************/
/*!
\file       King_Arthur.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the last boss king arthur

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Characters.h"
#include "Boss_Phase.h"
#include "Dragon.h"

class King_Arthur : public Characters
{
private:
	enum KA_MoveSet // for switch case to determine which attack gets called
	{
		SINGLE_SLASH = 0,
		TRIPLE_SLASH,
		DASH = 4,
		HEAL,
		SPIN_SWORD,
		NONE
	};

	void King_Arthur_Phase2(void); // changes mechanics for phase 2
	void King_Arthur_Phase3(void);
	void Init_KA_Attacks(void);    // initialize king arthur attacks
	void Init_MobArray(void);      // initialize the mob array for phase 2
	void Set_Forward_Dir(const Dragon&);
    
	
	// boss behaviours /////////////////////////////////////////////////
	void Idle(const Dragon &d, const float dt);
	void Moving(const Dragon &d, const float dt);
	void Attack(Dragon &d, const float dt);
	void Dead(void);
	// boss behaviours end /////////////////////////////////////////////

	// attack functions /////////////////////////////////////////////////
	void Dash_Attack(Dragon &d, const float dt);     // jump attack
	void Single_Slash(Dragon &d, const float dt);    // single slash + wave
	void Triple_Slash(Dragon &d, const float dt);    // single slash 3 times
	void Heal_and_Spawn(Dragon &d, const float dt);  // phase 2, heal and spawm mobs
    void Spinning_Blades(Dragon &d, const float dt); // rotating swords that attacks player
	// attack functions /////////////////////////////////////////////////

	BOSS_PHASE ka_phase;
	Particle_System *healing_effect;
	KA_MoveSet currAttk;

public:
	/******************************************************************************/
	/*!
	\fn         King Arthuer

	\brief      Constructor for King Arthur

	\param      Sprite&& t_sprite
	- an rvalue of the sprite to be moved

	\retval     void
	No return.
	*/
	/******************************************************************************/
	King_Arthur(Sprite*);

	~King_Arthur(); // destructor
	void Update(Dragon &player, const float dt);
	void Render();

};

