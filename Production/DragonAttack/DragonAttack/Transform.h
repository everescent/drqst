/* Start Header ************************************************************************/
/*!
\file    Transform.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Transform class declared here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"

/****************************************************************************************
Description:
  Transform holds the three transformation matrices needed for an object. 
  These matrices are: 
  Translation, 
  Rotation, 
  Scale. 
Constructors:
  Sets all matrices to the Identity Matrix. 
Deconstructor:
  None. 
****************************************************************************************/
class Transform {
public:
  /**************************************************************************************
  Description:
    Sets the translation matrix and applies it to the matrix. 
  posX: 
    Position X. 
  posY:
    Position Y. 
  **************************************************************************************/
  void    SetTranslate(float posX,   float posY  );
  /**************************************************************************************
  Description:
    Sets the degrees to rotate and applies it to the rotation matrix. 
  Deg:
    Angle in Degrees. 
  **************************************************************************************/
  void    SetRotation (const float Deg           );
  /**************************************************************************************
  Description:
    Sets the scale matrix and applies it to the scale matrix. 
  scaleX:
   Horizontal scale. 
  scaleY:
    Vertical scale.
  **************************************************************************************/
  void    SetScale    (float scaleX, float scaleY);
  /**************************************************************************************
  Description:
    Concatenates all 3 matrices (Translate, Rotate, Scale) into a resultant matrix. 
  **************************************************************************************/
  void    Concat() 
  { 
    AEMtx33Concat(&Matrix, &R_Matrix, &S_Matrix);
    AEMtx33Concat(&Matrix, &T_Matrix, &Matrix  );
  }
  /**************************************************************************************
  Description:
    Returns the determinant of the resultant matrix. 
  **************************************************************************************/
  float   GetDeterminant()           { return AEMtx33Determinant(&Matrix); }
  /**************************************************************************************
  Description:
    Returns the degree of rotation. 
  **************************************************************************************/
  float   GetDegree()          const { return Degree;                      }
  /**************************************************************************************
  Description:
    Returns the resultant matrix. 
  **************************************************************************************/
  AEMtx33 GetMatrix()          const { return Matrix;                      }
  /**************************************************************************************
  Description:
    Returns the translation matrix. 
  **************************************************************************************/
  AEMtx33 GetTranslateMatrix() const { return T_Matrix;                    }
  /**************************************************************************************
  Description:
    Returns the scale matrix 
  **************************************************************************************/
  AEMtx33 GetScaleMatrix()     const { return T_Matrix;                    }
  /**************************************************************************************
  Description:
    Sets all matrices to Identity Matrices. 
  **************************************************************************************/
  Transform()
  :Matrix  { 0.0f }, T_Matrix{ 0.0f }, S_Matrix{ 0.0f }, 
   R_Matrix{ 0.0f }, Degree  { 0.0f }
  {
    //Set all matrices to Identity Matrices
    AEMtx33Identity( &Matrix   );
    AEMtx33Identity( &T_Matrix );
    AEMtx33Identity( &R_Matrix );
    AEMtx33Identity( &S_Matrix );
  }
private:
  AEMtx33 Matrix  ; //Resultant matrix
  AEMtx33 T_Matrix; //Translation matrix
  AEMtx33 R_Matrix; //Scale matrix
  AEMtx33 S_Matrix; //Scale matrix
  float   Degree  ; //Rotation in degrees
};
