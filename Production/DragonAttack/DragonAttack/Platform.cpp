#include "Platform.h"

Platform::Platform()
	: GameObject{ CreatePlatform(1.0f, 1.0f, 1.0f, 1.0f, ".//Textures/download.jpg"),
	  Col_Comp{ 0.0f - PLAT_WIDTH, 0.0f - PLAT_HEIGHT, 
              0.0f + PLAT_WIDTH, 0.0f + PLAT_HEIGHT, Rect }, 
              0.0f, -100.0f }
{
	SetActive(true);
}
//(Jacob) Updated to make dragon stand on platform
void Platform::Update(Dragon &player, const float &dt)
{
  //(Jacob) So that I don't have to check for collision twice
  bool Colide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), 
                player.GetVelocity(), dt);
  //(Jacob) Checks for collision, and have player stand on platform if true
  //(Jacob) Make sure player doesn't go through the platform
  if (Colide && player.PosY < PosY)
  {
    if (player.PosX < PosX)
      player.PosX = PosX - Sprite_.Get_Width() - 100.0f;
    else if (player.PosX > PosX)
      player.PosX = PosX + Sprite_.Get_Width() + 100.0f;
  }
  //(Jacob) Make sure player is on the platform if colide
  if (Colide && !player.GetJump())
  {
    if(player.PosY <= PosY + Sprite_.Get_Height() + 100.0f)
      player.PosY = PosY + Sprite_.Get_Height() + 100.0f;
  }
  //(Jacob) Changed to PosX and PosY
  this->Transform_.SetTranslate(PosX, PosY);
  this->Transform_.Concat();
}
