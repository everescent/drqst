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
	Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}

void Scarecrow::Update(Dragon &d, const float dt)
{
	if (this->IsActive() == true)
	{
		this->PosY -= 10.0f; //testing gravity

		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();


		this->Collision_.Update_Col_Pos(this->PosX - SC_SCALE, this->PosY - SC_SCALE,  // min point
		this->PosX + SC_SCALE, this->PosY + SC_SCALE); // max point


		for (char i = 0; i < Bullet_Buffer; ++i)
			if (d.GetFireball()[i].IsActive())
				if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, this->GetVelocity(), d.GetFireball()[i].GetVelocity(), dt))
				{
					//Decrease_HP(d.GetDamage());
					d.AddCharge();
					SetActive(false);
					d.GetFireball()[i].Projectile::ResetDist();
					d.GetFireball()[i].SetActive(false);
				}

	}
}

