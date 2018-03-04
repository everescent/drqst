/* Start Header ************************************************************************/
/*!
\file       Collision.cpp
\author     Javon Lee
\par email: xiongweijavon.lee\@digipen.edu
\brief
Collision class functions are defined here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Collision.h" 
#include <iostream> // for debugging
bool Col_Comp::St_Rect_Rect( const Col_Comp &aabb_obj2)
{
	//check if the two objects are Rectangles 

	if (shape != Rect || aabb_obj2.shape != Rect)
	{
		return false; // no collision if wrong function used. 
	}


	f32 left_A = min.x, left_B = aabb_obj2.min.x;
	f32 right_A = max.x, right_B = aabb_obj2.max.x;
	f32 top_A = max.y, top_B = aabb_obj2.max.y;
	f32 btm_A = min.y, btm_B = aabb_obj2.min.y;


	//Check for NO collision using AABB between 2 objects 
	if (left_A > right_B) return false;
	if (left_B > right_A) return false;
	if (top_A < btm_B) return false;
	if (top_B < btm_A) return false;
	//else return false

	return true;
}

bool Col_Comp::St_Circle_Circle( const Col_Comp &objB)
{
	int RadiusSumSq = (radius + objB.radius)*(radius + objB.radius);
	f32 DistCircleSq = (mid.x - objB.mid.x)*(mid.x - objB.mid.x) + (mid.y - objB.mid.y)*(mid.y - objB.mid.y);

	if (DistCircleSq <= RadiusSumSq) return true; // There's collision

	return false;

}

bool Col_Comp::St_Rect_Point(float point_x, float point_y)
{

	if (point_x >= min.x && point_x <= max.x && point_y >= min.y && point_y <= max.y ) return true;

	return false;
}

bool Col_Comp::Dy_Rect_Rect( const Col_Comp &B, const AEVec2 &velocity_A, const AEVec2 &velocity_B, float dt)
{
	
	float Vel_x_A = velocity_A.x;
	float Vel_x_B = velocity_B.x;
	float Vel_y_A = velocity_A.y;
	float Vel_y_B = velocity_B.y;
	float Vel_x_Res = Vel_x_B - Vel_x_A;
	float Vel_y_Res = Vel_y_B - Vel_y_A;
	float t_first = 0, t_first_x = 0 , t_first_y = 0 ;
	float t_last = dt, t_last_x = 0, t_last_y = 0;
	
	f32 left_A = min.x, left_B = B.min.x;
	f32 right_A = max.x, right_B = B.max.x;
	f32 top_A = max.y, top_B = B.max.y;
	f32 btm_A = min.y, btm_B = B.min.y;


	//Check for NO collision using AABB between 2 objects 
	if (left_A > right_B) return false;
	if (left_B > right_A) return false;
	if (top_A < btm_B) return false;
	if (top_B < btm_A) return false;

	if (Vel_x_Res < 0)
	{
		//Case 1
		if (this->min.x > B.max.x) // object B on the left of A 
		{
			return false;
		}

		//Case 4 
		if (this->max.x < B.min.x) // object B on the right of A 
		{
			t_first_x = (this->max.x - B.min.x) / Vel_x_Res;
			t_last_x = (this->min.x - B.max.x) / Vel_x_Res;
		}

	}

	if (Vel_x_Res > 0)
	{
		//Case 2
		if (this->min.x > B.max.x) // object B on the left of A 
		{
			t_first_x = (this->min.x - B.max.x) / Vel_x_Res;
			t_last_x = (this->max.x - B.min.x) / Vel_x_Res;
		}
		//Case 3
		if (this->max.x < B.min.x) // object B on the right of A 
		{
			return false;
		}
	}
	
	//--------------------------------------Repeat Step for Y 

	if (Vel_y_Res < 0)
	{
		//Case 1
		if (this->min.y > B.max.y) //object B is below A 
		{
			return false;
		}

		//Case 4 
		if (this->max.y < B.min.y) // object B on the right of A 
		{
			t_first_y = (this->max.y - B.min.y) / Vel_y_Res;
			t_last_y = (this->min.y - B.max.y) / 
				Vel_y_Res;
		}

	}

	if (Vel_y_Res > 0)
	{
		//Case 2

		if (this->min.x > B.max.x) // object B on the left of A 
		{
			t_first_y = (this->min.y - B.max.y) / Vel_y_Res;
			t_last_y = (this->max.y - B.min.y) / Vel_y_Res;
		}
		//Case 3
		if (this->max.x < B.min.x) // object B is on top of A 
		{
			return false;
		}
	}
	
	//---------------check for correct t_first and t_last 

	t_first = t_first_x > t_first_y ? t_first_x : t_first_y;

	t_last = t_last_x < t_last_y ? t_last_x : t_last_y; 

	//Case 5
	if (t_first > t_last)
	{
		return false; 
	}

	return true; 

}

void Col_Comp::Update_Col_Pos(f32 min_x, f32 min_y, f32 max_x, f32 max_y)
{
	min.x = min_x;
	min.y = min_y;
	max.x = max_x;
	max.y = max_y;

}

void Col_Comp::Update_Col_Pos(f32 mid_x, f32 mid_y)
{
	mid.x = mid_x;
	mid.y = mid_y;
}

Shape Col_Comp::GetShape()
{
	return shape;
}


bool Col_Comp::Line_Point(Col_Comp &A, Col_Comp &B,  AEVec2 & A_Pos)
{
	float line_dist  = AEVec2SquareDistance(&A.mid, &A_Pos);
	float start_dist = AEVec2SquareDistance(&A.mid, &B.max);
	float end_dist   = AEVec2SquareDistance(&A_Pos, &B.max);
	float buffer = 0.1f;


	//std::cout << line_dist << std::endl;
	//std::cout << start_dist + end_dist << std::endl;
	//std::cout << A_Pos.x <<  " " << A_Pos.y<<std::endl;
	//std::cout << "\n";

	if (start_dist + end_dist >= line_dist - buffer && start_dist + end_dist <= line_dist + buffer)
	{
		return true;
	}

		return false;
}

AEVec2 Col_Comp::Get_Point(void) const
{
	return mid;
}

