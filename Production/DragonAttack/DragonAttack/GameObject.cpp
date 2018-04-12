/* Start Header ************************************************************************/
/*!
\file    GameObject.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Game Object member functions defined here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "AEEngine.h"
#include "GameObject.h"
#include <utility> //move

//Renders the object
void GameObject::Render()
{
  //Renders the object if active
  if(active)
    Sprite_->Render_Object(Transform_);
}

//Default constructor; Initializes to zero
GameObject::GameObject()
:active{ false }, Transform_{}, Sprite_{}
{}

//Move constructor for sprite
GameObject::GameObject(Sprite* p_Sprite, Col_Comp &&t_Col, float x, float y)
:active { false    }, Velocity  { 0.0f, 0.0f       }, Transform_{}, 
 Sprite_{ p_Sprite }, Collision_{ std::move(t_Col) },
 PosX   { x        }, PosY      { y                }
{}
