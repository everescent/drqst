#include "Platform.h"
#include "Transform.h"
#include <utility>

Platform::Platform()
	: GameObject{ CreatePlatform(2.5f, 1.0f, 1.0f, 1.0f, ".//Textures/download.jpg"),
	Col_Comp{ 0.0f, 0.0f, 5.0f, 5.0f, Rect } }
{
	SetActive(true);
}

void Platform::Update(float X, float Y)
{
	this->Transform_.SetTranslate(X, Y);
	this->Transform_.Concat();
}