#include "Platform.h"
#include <iostream>

Platform::Platform(float x, float y)
	: GameObject{ CreatePlatform(1.5f, 1.0f, ".//Textures/download.jpg"),
	Col_Comp{ x - PLAT_WIDTH, y - PLAT_HEIGHT,
	x + PLAT_WIDTH, y + PLAT_HEIGHT, Rect },
	x, y }
{
	SetActive(true);
	PosX = x;
	PosY = y;
}
//(Jacob) Updated to make dragon stand on platform
void Platform::Update(Dragon &player, const float &dt)
{
	//(Jacob) So that I don't have to check for collision twice
	bool Colide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(),
		player.GetVelocity(), dt);
	//(Jacob) Changed to PosX and PosY
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
	//(Jacob) Make sure player is on the platform if colide
	if (Colide && !player.GetJump())
	{
		if (player.PosY - player.Sprite_.Get_Height() < PosY)
			return;
		if (player.PosY <= PosY + Sprite_.Get_Height() + player.Sprite_.Get_Height())
			player.PosY = PosY + Sprite_.Get_Height() + player.Sprite_.Get_Height();
	}
}


//(Jacob) Updated to make dragon stand on platform
void Platform::Update(Characters &player, const float &dt, const bool xSnapping)
{
  //(Jacob) So that I don't have to check for collision twice
  bool Colide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(),
                player.GetVelocity(), dt);
  //(Jacob) Changed to PosX and PosY
  this->Transform_.SetTranslate(PosX, PosY);
  this->Transform_.Concat();
  //(Jacob) Make sure player is on the platform if colide
  if (Colide)
  {
    if (player.PosY - player.Sprite_.Get_Height() < PosY)
    {
      //Do nothing
    }
    else if (player.PosY <= PosY + Sprite_.Get_Height() + player.Sprite_.Get_Height())
      player.PosY = PosY + Sprite_.Get_Height() + player.Sprite_.Get_Height();
  }
  //(Jacob) Checks for collision, and have player stand on platform if true
  //(Jacob) Make sure player doesn't go through the platform
  if (Colide && player.PosY > PosY && xSnapping)
  {
    if (player.PosX < PosX - Sprite_.Get_Width())
      player.PosX = PosX - Sprite_.Get_Width();
    else if (player.PosX > PosX + Sprite_.Get_Width())
      player.PosX = PosX + Sprite_.Get_Width();
  }
}
