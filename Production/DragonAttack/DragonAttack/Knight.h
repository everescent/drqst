#pragma once
#include "Characters.h"
#include "Dragon.h"
#include "Boss_States.h"

class Knight : public Characters
{
private:
	Boss_Action_State current_action;
	Boss_Attack stab;
	float time_traveled;
	Animation anim;

	void Idle(const Dragon&, const float dt);
	void Moving(const Dragon&, const float dt);
	void Attack(Dragon&, const float dt);
	void Dead(void);

	void Set_Forward_Dir(const Dragon&);
	bool Line_Of_Sight(const Dragon&);

public:
	Knight(const AEVec2&, Sprite* );
	void Update(Dragon&, const float dt);
	void Render(void);

};