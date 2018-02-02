#include "Boss_States.h"

void Boss_Attack::Update(float dt)  //has to check cooldown_timer
{
	if (cooldown)
	{
		cooldown_timer -= dt;
	}
}
void Boss_Attack::Pos()
{
	//calculate the position and shit for attack

	this->Render();

	if (this->GetDir())
		this->PosX += this->GetVelocity().x;
	else
		this->PosX -= this->GetVelocity().x;

	this->AddDist (this->GetVelocity().x);
	this->Transform_.SetTranslate(this->PosX, 0.0f);

}