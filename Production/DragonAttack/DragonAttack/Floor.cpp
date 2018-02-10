#include "Floor.h"
#include "Transform.h"
#include <utility>

Floor::Floor()
	: GameObject{ CreateFloor(1.0f, 1.0f, 1.0f, ".//Textures/download.jpg"),
	Col_Comp{ 0.0f, 0.0f, 5.0f, 5.0f, Rect } }
{
	SetActive(true);
}

void Floor::Update(float X, float Y)
{
	this->Transform_.SetTranslate(X, Y);
	this->Transform_.Concat();
}