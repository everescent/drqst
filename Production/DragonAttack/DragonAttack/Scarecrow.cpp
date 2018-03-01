#include "Scarecrow.h"
#include "Transform.h"
#include "Physics.h"
#include <iostream>

namespace // global variables just for THIS file
{
	const int scarecrow_hp = 1;
	const float SC_SCALE = 120.0f;
}

Scarecrow::Scarecrow(float x, float y)
	: Characters(S_CreateSquare(SC_SCALE, ".//Textures/scarecrow.png"),
		scarecrow_hp,
		Col_Comp{ x - SC_SCALE, y - SC_SCALE , x + SC_SCALE , y + SC_SCALE, Rect })
{
	SetActive(true);
	PosX = x;
	PosY = y;
}

void Scarecrow::Update(Dragon &d, const float dt)
{
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	for (char i = 0; i < Bullet_Buffer; ++i)
		if (d.GetFireball()[i].IsActive())
			if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, this->GetVelocity(), d.GetFireball()[i].GetVelocity(), dt))
			{
				Decrease_HP(d.GetDamage());
				//SetActive false here
				d.GetFireball()[i].Projectile::ResetDist();
				d.GetFireball()[i].SetActive(false);
			}
}

