#include "AEEngine.h"
#include "GameObject.h"
#include <utility>

void GameObject::Render()
{
  Sprite_.Render_Object(Transform_);
}

GameObject::GameObject()
  :active{ false }, Transform_{}, Sprite_{}
{}

GameObject::GameObject(AEGfxVertexList *mesh, const float &ObjectW, const float &ObjectH)
  : active{ false }, Transform_{}, Sprite_{ mesh, ObjectW, ObjectH }
{}

GameObject::GameObject(AEGfxVertexList * mesh, const char* TexFile,
                       float ObjectW, const float &ObjectH)
  : active{ false }, Transform_{}, Sprite_{ mesh, TexFile, ObjectW, ObjectH }
{}
GameObject::GameObject(Sprite&& t_Sprite)
  : active{ false }, Transform_{}, Sprite_{ std::move(t_Sprite) }
{}
