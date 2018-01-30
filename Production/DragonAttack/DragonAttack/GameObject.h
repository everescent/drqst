
#pragma once
#include "AEEngine.h"
#include "Transform.h"
#include "Sprite.h"

class Transform;
class Sprite;

class GameObject {

public:
  virtual void Update() = 0;
  virtual void Pos() = 0;
  void Render();
  bool active;
  GameObject();
  GameObject(AEGfxVertexList *mesh, const float &ObjectW, const float &ObjectH);
  GameObject(AEGfxVertexList * mesh, const char* TexFile,
             float ObjectW, const float &ObjectH);
  GameObject(Sprite&& t_Sprite);
private:
  Transform Transform_;
  Sprite Sprite_;
};
