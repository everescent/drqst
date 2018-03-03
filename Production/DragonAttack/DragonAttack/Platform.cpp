#include "Platform.h"
#include <iostream>
Platform::Platform(float x, float y)
	: GameObject{ CreatePlatform(1.5f, 1.0f, ".//Textures/download.jpg"),
	  Col_Comp{ x - PLAT_WIDTH, y - PLAT_HEIGHT, 
                x + PLAT_WIDTH, y + PLAT_HEIGHT, Rect }, 
                x, y }
{
	SetActive(true);
}
//(Jacob) Updated to make dragon stand on platform
void Platform::Update(Dragon &player, const float &dt)
{
	//(Jacob) Changed to PosX and PosY
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

  //(Jacob) So that I don't have to check for collision twice
  bool Colide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), 
                player.GetVelocity(), dt);
  //(Jacob) Changed to PosX and PosY
  this->Transform_.SetTranslate(PosX, PosY);
  this->Transform_.Concat();
  //(Jacob) Checks for collision, and have player stand on platform if true
  //(Jacob) Make sure player doesn't go through the platform
 // if (Colide && player.PosY < PosY && !player.GetJump())
 // {
	//if (player.PosY < PosY)
	//	  return;
 //   if (player.PosX < PosX)
 //     player.PosX = PosX - Sprite_.Get_Width() - 100.0f;
 //   else if (player.PosX > PosX)
 //     player.PosX = PosX + Sprite_.Get_Width() + 100.0f;
 // }
  //(Jacob) Make sure player is on the platform if colide
  if (Colide && !player.GetJump())
  {
	if (player.PosY - 70.0f < PosY)
		return;
    if(player.PosY <= PosY + Sprite_.Get_Height() + 70.0f)
      player.PosY = PosY + Sprite_.Get_Height() + 70.0f;
  }

  
}
