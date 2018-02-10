#include "Wall.h"
#include "Transform.h"
#include <utility>

Wall::Wall()
	: GameObject{ S_CreateRectangle(50.0f, 200.0f, 1.0f, 1.0f, ".//Textures/download.jpg"),
	Col_Comp{ 0.0f, 0.0f, 5.0f, 5.0f, Rect } }
{
	SetActive(true);
}

void Wall::Update(float X, float Y)
{
	this->Transform_.SetTranslate(X, Y);
	this->Transform_.Concat();
}