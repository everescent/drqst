/* Start Header ************************************************************************/
/*!
\file    Sprite.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Sprite class declared here. 

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Transform.h"
#include "Render.h"

const float FLOOR_WIDTH { 40.0f  }; //The half width of the floor
const float FLOOR_HEIGHT{ 45.0f  }; //The half height of the floor
const float PLAT_WIDTH  { 45.0f  }; //The half width of one platform
const float PLAT_HEIGHT { 20.0f  }; //The half height of one platform
const float BG_WIDTH    { 640.0f }; //The half width of one background
const float BG_HEIGHT   { 360.0f }; //The half height of one background

/****************************************************************************************
Description: 
  Creates an object.
Constructors:
  Object can be constructed with a mesh or with mesh and texture or none
Deconstructor:
  Object will free its resources when done
****************************************************************************************/
class Sprite {

public:
  /**************************************************************************************
  Description:
    Set texture position.
  posU:
    Position U.
  posV:
    Position V.
  **************************************************************************************/
  Sprite& SetTexPos       (const float posU       , const float  posV        );

  Sprite& SetTex(AEGfxTexture* texture);

  /**************************************************************************************
  Description:
    Set tint RGB.
  Red:
    Red value.
  Green:
    Green value.
  Blue:
    Blue value.
  **************************************************************************************/
  Sprite& SetRGB          (const float  Red = 1.0f, const float Green = 1.0f,
                           const float Blue = 1.0f                           );

  /**************************************************************************************
  Description:
    Set Alpha, Transparency, Blend Mode.
  Alpha:
    Alpha value.
  Trans:
    Transparency value.
  BlendMode:
    Blend mode.
  **************************************************************************************/
  Sprite& SetAlphaTransBM (const float  Alpha     , const float Trans       ,
                           AEGfxBlendMode BlendMode                          );

  /**************************************************************************************
  Description:
    Renders the object.
  matrix:
    Transformation matrix of object. 
  **************************************************************************************/
  void             Render_Object(const Transform &matrix)                const;
  /**************************************************************************************
  Description:
    Returns position U of UV.
  **************************************************************************************/
  float            Get_posV     () const { return V     ; }
  /**************************************************************************************
  Description:
    Returns position V of UV.
  **************************************************************************************/
  float            Get_posU     () const { return U     ; }
  /**************************************************************************************
  Description:
    Returns width of object.
  **************************************************************************************/
  float            Get_Width    () const { return Width ; }
  /**************************************************************************************
  Description:
    Returns height of object.
  **************************************************************************************/
  float            Get_Height   () const { return Height; }
  /**************************************************************************************
  Description:
    Returns mesh of object.
  **************************************************************************************/
  AEGfxVertexList* Get_mesh     () const { return Mesh  ; }
  /**************************************************************************************
  Description:
    Returns render mode of object.
  **************************************************************************************/
  AEGfxRenderMode  Get_rm       () const { return RM    ; }
  /**************************************************************************************
  Description:
    Returns texture of object.
  **************************************************************************************/
  AEGfxTexture*    Get_tex      () const { return Tex   ; }
  /**************************************************************************************
  Description:
    Returns the object.
  **************************************************************************************/
  Sprite&          Get_this     ()       { return *this ; }

  /**************************************************************************************
  Description:
    Default constructor; Sets everything to zero.
  **************************************************************************************/
  Sprite                   (                                                 );

  /**************************************************************************************
  Description:
    Move constructor.
  Move_Object:
    Object to move from.
  **************************************************************************************/
  Sprite                   (Sprite&& Move_Object                             );

  /**************************************************************************************
  Description:
    Move assignment operator.
  Move_Object:
    Object to move from.
  **************************************************************************************/
  Sprite& Sprite::operator=(Sprite&& Move_Object                             );

  /**************************************************************************************
  Description:
    Construct with a mesh, but no texture.
  mesh:
    Mesh of the object. 
  ObjectW:
    Object width.
  ObjectH:
    Object height.
  **************************************************************************************/
  Sprite                  (AEGfxVertexList * mesh,
                           const float    ObjectW,                           
                           const float    ObjectH                            );

  /**************************************************************************************
  Description:
    Construct with a mesh and texture.
  mesh:
    Mesh of the object.
  TexFile:
    Texture file of the object.
  ObjectW:
    Object width.
  ObjectH:
    Object height.
  **************************************************************************************/
  Sprite                  (AEGfxVertexList * mesh, const char* TexFile,      
                           const float    ObjectW, const float ObjectH       );

  /**************************************************************************************
  Description:
    Free all resources.
  **************************************************************************************/
  ~Sprite                 (                                                  );

private:
  AEGfxVertexList* Mesh  ; //A pointer to object's mesh
  AEGfxRenderMode  RM    ; //Render Mode
  AEGfxTexture*    Tex   ; //Texture
  Render           Draw  ; //Draw component
  float            U     ; //Texture U
  float            V     ; //Texture V
  float            Width ; //Width of object
  float            Height; //Height of object
};
