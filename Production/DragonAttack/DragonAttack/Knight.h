#pragma once
#include "Characters.h"
#include "Boss_States.h"

class Knight : public Characters
{
private:
	Boss_Action_State current_action;

	void Idle(const float dt);
	void Moving(const Dragon&, const float dt);
	void Attack(Dragon&);
	void Dead(void);

	bool Line_Of_Sight(const Dragon&);

public:
	Knight(const AEVec2& );
	void Update(Dragon&, const float dt);
	void Render(void);

};