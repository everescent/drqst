/* Start Header ************************************************************************/
/*!
\file       Collision.cpp
\project    Dragon Attack
\author     Javon Lee
\par email: xiongweijavon.lee\@digipen.edu
\brief
This file contains the Col_Comp (Collision Componenet) class declaration

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Transform.h"

// Meant to be shared to determine if you want which side of the ling you want to check
enum HalfPlane 
{
	INSIDE = 1,
	OUTSIDE
};

namespace
{
	enum Shape
	{
		NoShape,
		Point,
		Rect,
		Circle
	};

	enum CollisionFlag
	{
		Collided,
		Not_Collided
	};
}

/****************************************************************************************
Description:
This is a class that contains the collision componenet for the Game Objects
****************************************************************************************/
class Col_Comp
{
public:
	Col_Comp(f32 mid_x, f32 mid_y, Shape shape) // constructor for point
		: mid{ mid_x, mid_y }, shape{ shape }
	{
		if (shape != Point)
			shape = NoShape;
	}

	Col_Comp(f32 min_x, f32 min_y, f32 max_x, f32 max_y, Shape shape)
		: min{ min_x , min_y }, max{ max_x, max_y }, shape{ shape } 
	{
		if (shape != Rect)
			shape = NoShape;
	} //empty body constructor
/****************************************************************************************
Description:
This is a constructor for Circle shapes 
****************************************************************************************/
	Col_Comp(f32 mid_x, f32 mid_y, int radius, Shape shape) // constructor for circles 
		: mid{ mid_x, mid_y }, radius{ radius }, shape{ shape }
	{
		if (shape != Circle)
			shape = NoShape;
	}
/****************************************************************************************
Description:
This is a copy constructor that allows a Col_Comp to be constructed with a R-value Col_Comp
Example:
	//Col_Comp test{Col_Comp{1.0f,1.0f,10,Circle}};
	//OR
	//Col_Comp test{Col_Comp{0.0f,0.0f,5.0f5.0f,Rectangle}};
****************************************************************************************/
  Col_Comp(Col_Comp&& t_Col)
    :min{ t_Col.min.x, t_Col.min.y }, max{ t_Col.max.x, t_Col.max.y }, mid{ t_Col.mid.x, t_Col.mid.y },
    radius{ t_Col.radius }, shape{ t_Col.shape }, colflag{ t_Col.colflag }
  {}//Empty constructor body
  

/****************************************************************************************
Description:
This is a copy constructor that allows a Col_Comp to be constructed with a R-value Col_Comp
Example:
//Col_Comp test{Col_Comp{1.0f,1.0f,10,Circle}};
//OR
//Col_Comp test{Col_Comp{0.0f,0.0f,5.0f5.0f,Rectangle}};
****************************************************************************************/
	Col_Comp() {} // call this constructor for non-collidable objects
/****************************************************************************************
Description:
This is a destructor for the Collision Component class
****************************************************************************************/
	~Col_Comp() {}; //default dstor
/****************************************************************************************
Description:
This function is used to update the collision position of objects when it is moving 
****************************************************************************************/
	void Update_Col_Pos(f32 min_x, f32 min_y, f32 max_x, f32 max_y); // update for rectangle objects 
/****************************************************************************************
Description:
This function is used to update the collision position of objects when it is moving
****************************************************************************************/
	void Update_Col_Pos(f32 mid_x, f32 mid_y); //update for Circle objects 
/****************************************************************************************
Description:
This function checks for static collision for Rectangle to Rectangle
****************************************************************************************/
	bool St_Rect_Rect( const Col_Comp &aabb_obj2);
/****************************************************************************************
Description:
This function checks for static collision for Circle to Circle
****************************************************************************************/
	bool St_Circle_Circle( const Col_Comp &aabb_obj2); //static collision for Circle to Circle
/****************************************************************************************
Description:
This function checks for static collision for Rectangle to a point
****************************************************************************************/
	bool St_Rect_Point(float point_x, float point_y);
/****************************************************************************************
Description:
This function checks for dynamic collision for Rectangle to Rectangle
****************************************************************************************/
	bool Dy_Rect_Rect(const Col_Comp &B, const AEVec2 &velocity_A, const AEVec2 &velocity_B, float dt);
/****************************************************************************************
Description:
This function is to get the shape of an Object
****************************************************************************************/
	Shape GetShape(); 
/****************************************************************************************
Description:
This function checks for dynamic collision between a line and a point 
****************************************************************************************/
	bool Line_Point(Col_Comp &A, Col_Comp &B, AEVec2 & A_Pos, HalfPlane);
/****************************************************************************************
Description:
This function checks for static collision for Point to Rectangle
****************************************************************************************/
	bool Point_Rect(Col_Comp &A, Col_Comp &B);
/****************************************************************************************
Description:
This function Gets the point of an object
****************************************************************************************/
	AEVec2 Get_Point(void) const;
/****************************************************************************************
Description:
This function gets the maximum point of a bounding box
****************************************************************************************/
    AEVec2 Get_MaxPoint(void) const;
/****************************************************************************************
Description:
This function gets the minimum point of a bounding box
****************************************************************************************/
    AEVec2 Get_MinPoint(void) const;
	
private:

	AEVec2 min = { 0, 0 }; //represents btm left of a rect/sq

	AEVec2 max = { 0, 0 }; //represents top right of a rect/sq

	AEVec2 mid = { 0, 0 }; //represents the mid of a circle 

	int radius = 0; //represents the radius of a circle 

	Shape shape = NoShape;

	AEVec2 velocity = { 0, 0 }; //represents the current position of the object 

	CollisionFlag colflag = Not_Collided;

};




