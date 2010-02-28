/*
   Base Render Library   
   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
 */

#include "Vector2D.h"

namespace Math
{
	//-------------------------------------- Public Constants -------------------------------------

	const Vector2D Vector2D :: Zero ( 0.0F, 0.0F );

	const Vector2D Vector2D :: Unit ( 1.0F, 1.0F );

	const Vector2D Vector2D :: AxisX ( 1.0F, 0.0F );

	const Vector2D Vector2D :: AxisY ( 0.0F, 1.0F );

	//--------------------------------------- Constructors ----------------------------------------

	Vector2D :: Vector2D ( float x, float y )
	{
		X = x;

		Y = y;
	}

	Vector2D :: Vector2D ( const float vector [SIZE2D] )
	{
		X = vector [A];

		Y = vector [B];
	}

	Vector2D :: Vector2D ( const Vector2D& vector )
	{
		X = vector [A];

		Y = vector [B];
	}

	Vector2D :: Vector2D ( const Vector3D& vector )
	{
		X = vector [A];

		Y = vector [B];
	}

	Vector2D :: Vector2D ( const Vector4D& vector )
	{
		X = vector [A];

		Y = vector [B];
	}

	//------------------------------------ Arithmetic Operators -----------------------------------

	Vector2D operator - ( const Vector2D& source )
	{
		return Vector2D ( -source.X, -source.Y );
	}

	Vector2D operator + ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X + right.X, left.Y + right.Y );
	}

	Vector2D operator - ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X - right.X, left.Y - right.Y );
	}

	Vector2D operator + ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X + right, left.Y + right );
	}

	Vector2D operator - ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X - right, left.Y - right );
	}

	Vector2D operator + ( float left, const Vector2D& right )
	{
		return Vector2D ( left + right.X, left + right.Y );
	}

	Vector2D operator - ( float left, const Vector2D& right )
	{
		return Vector2D ( left - right.X, left - right.Y );
	}

	Vector2D operator * ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X * right.X, left.Y * right.Y );
	}

	Vector2D operator / ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X / right.X, left.Y / right.Y );
	}

	Vector2D operator * ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X * right, left.Y * right );
	}

	Vector2D operator / ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X / right, left.Y / right );
	}

	Vector2D operator * ( float left, const Vector2D& right )
	{
		return Vector2D ( left * right.X, left * right.Y );
	}

	Vector2D operator / ( float left, const Vector2D& right )
	{
		return Vector2D ( left / right.X, left / right.Y );
	}

	Vector2D operator += ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X += right.X, left.Y += right.Y );
	}

	Vector2D operator -= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X -= right.X, left.Y -= right.Y );
	}

	Vector2D operator += ( Vector2D& left, float right )
	{
		return Vector2D ( left.X += right, left.Y += right );
	}

	Vector2D operator -= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X -= right, left.Y -= right );
	}

	Vector2D operator *= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X *= right.X, left.Y *= right.Y );
	}

	Vector2D operator /= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X /= right.X, left.Y /= right.Y );
	}

	Vector2D operator *= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X *= right, left.Y *= right );
	}

	Vector2D operator /= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X /= right, left.Y /= right );
	}

	//-------------------------------------- Logic Operators --------------------------------------

	bool operator < ( const Vector2D& left, const Vector2D& right )
	{
		return left.X < right.X && left.Y < right.Y;
	}

	bool operator > ( const Vector2D& left, const Vector2D& right )
	{
		return left.X > right.X && left.Y > right.Y;
	}

	bool operator < ( const Vector2D& left, float right )
	{
		return left.X < right && left.Y < right;
	}

	bool operator > ( const Vector2D& left, float right )
	{
		return left.X > right && left.Y > right;
	}

	bool operator < ( float left, const Vector2D& right )
	{
		return left < right.X && left < right.Y;
	}

	bool operator > ( float left, const Vector2D& right )
	{
		return left > right.X && left > right.Y;
	}

	bool operator <= ( const Vector2D& left, const Vector2D& right )
	{
		return left.X <= right.X && left.Y <= right.Y;
	}

	bool operator >= ( const Vector2D& left, const Vector2D& right )
	{
		return left.X >= right.X && left.Y >= right.Y;
	}

	bool operator <= ( const Vector2D& left, float right )
	{
		return left.X <= right && left.Y <= right;
	}

	bool operator >= ( const Vector2D& left, float right )
	{
		return left.X >= right && left.Y >= right;
	}

	bool operator <= ( float left, const Vector2D& right )
	{
		return left <= right.X && left <= right.Y;
	}

	bool operator >= ( float left, const Vector2D& right )
	{
		return left >= right.X && left >= right.Y;
	}

	bool operator == ( const Vector2D& left, const Vector2D& right )
	{
		return left.X == right.X && left.Y == right.Y;
	}

	bool operator != ( const Vector2D& left, const Vector2D& right )
	{
		return left.X != right.X || left.Y != right.Y;
	}

	bool operator == ( const Vector2D& left, float right )
	{
		return left.X == right && left.Y == right;
	}

	bool operator != ( const Vector2D& left, float right )
	{
		return left.X != right || left.Y != right;
	}

	bool operator == ( float left, const Vector2D& right )
	{
		return left == right.X && left == right.Y;
	}

	bool operator != ( float left, const Vector2D& right )
	{
		return left != right.X || left != right.Y;
	}

	//-------------------------------- Input and Output Operators ---------------------------------

	istream& operator >> ( istream& is, Vector2D& source )
	{
		return is >> source.X >> source.Y;
	}

	ostream& operator << ( ostream& os, const Vector2D& source )
	{
		return os << source.X << " " << source.Y;
	}

	//------------------------------------- Common Functions --------------------------------------

	Vector2D Abs ( const Vector2D& source )
	{
		return Vector2D ( fabsf ( source.X ), fabsf ( source.Y ) );
	}

	Vector2D Sign ( const Vector2D& source )
	{
		return Vector2D ( Sign ( source.X ), Sign ( source.Y ) );
	}

	Vector2D Floor ( const Vector2D& source )
	{
		return Vector2D ( floorf ( source.X ), floorf ( source.Y ) );
	}

	Vector2D Ceil ( const Vector2D& source )
	{
		return Vector2D ( ceilf ( source.X ), ceilf ( source.Y ) );
	}

	Vector2D Fract ( const Vector2D& source )
	{
		return Vector2D ( Fract ( source.X ), Fract ( source.Y ) );
	}

	Vector2D Mod ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( Mod ( left.X, right.X ), Mod ( left.Y, right.Y ) );
	}

	Vector2D Mod ( const Vector2D& left, float right )
	{
		return Vector2D ( Mod ( left.X, right ), Mod ( left.Y, right ) );
	}

	Vector2D Mod ( float left, const Vector2D& right )
	{
		return Vector2D ( Mod ( left, right.X ), Mod ( left, right.Y ) );
	}

	Vector2D Min ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X < right.X ? left.X : right.X,
						  left.Y < right.Y ? left.Y : right.Y );
	}

	Vector2D Min ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X < right ? left.X : right,
						  left.Y < right ? left.Y : right );
	}

	Vector2D Min ( float left, const Vector2D& right )
	{
		return Vector2D ( left < right.X ? left : right.X,
						  left < right.Y ? left : right.Y );
	}

	Vector2D Max ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X > right.X ? left.X : right.X,
						  left.Y > right.Y ? left.Y : right.Y );
	}

	Vector2D Max ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X > right ? left.X : right,
						  left.Y > right ? left.Y : right );
	}

	Vector2D Max ( float left, const Vector2D& right )
	{
		return Vector2D ( left > right.X ? left : right.X,
						  left > right.Y ? left : right.Y );
	}

	Vector2D Clamp ( const Vector2D& source, const Vector2D& min, const Vector2D& max )
	{
		return Vector2D ( Clamp ( source.X, min.X, max.X ), Clamp ( source.Y, min.Y, max.Y ) );
	}

	Vector2D Clamp ( const Vector2D& source, float min, float max )
	{
		return Vector2D ( Clamp ( source.X, min, max ), Clamp ( source.Y, min, max ) );
	}

	Vector2D Mix ( const Vector2D& left, const Vector2D& right, const Vector2D& alpha )
	{
		return Vector2D ( Mix ( left.X, right.X, alpha.X ), Mix ( left.Y, right.Y, alpha.Y ) );
	}

	Vector2D Mix ( const Vector2D& left, const Vector2D& right, float alpha )
	{
		return Vector2D ( Mix ( left.X, right.X, alpha ), Mix ( left.Y, right.Y, alpha ) );
	}

	Vector2D Step ( const Vector2D& source, const Vector2D& value )
	{
		return Vector2D ( Step ( source.X, value.X ), Step ( source.Y, value.Y ) );
	}

	Vector2D Step ( const Vector2D& source, float value )
	{
		return Vector2D ( Step ( source.X, value ), Step ( source.Y, value ) );
	}

	Vector2D Smooth ( const Vector2D& source, const Vector2D& min, const Vector2D& max )
	{
		return Vector2D ( Smooth ( source.X, min.X, max.X ), Smooth ( source.Y, min.Y, max.Y ) );
	}

	Vector2D Smooth ( const Vector2D& source, float min, float max )
	{
		return Vector2D ( Smooth ( source.X, min, max ), Smooth ( source.Y, min, max ) );
	}

	//------------------------------------ Geometric Functions ------------------------------------

	float Length ( const Vector2D& source )
	{
		return sqrtf ( source.X * source.X + source.Y * source.Y );
	}

	float Square ( const Vector2D& source )
	{
		return source.X * source.X + source.Y * source.Y;
	}

	float Distance ( const Vector2D& left, const Vector2D& right )
	{
		return Length ( left - right );
	}

	float Dot ( const Vector2D& left, const Vector2D& right )
	{
		return left.X * right.X + left.Y * right.Y;
	}        

	Vector2D Normalize ( const Vector2D& source )
	{
		return source / Length ( source );
	}

	Vector2D Reflect ( const Vector2D& incident, const Vector2D& normal )
	{
		return incident - 2.0F * Dot ( normal, incident ) * normal;
	}

	Vector2D Refract ( const Vector2D& incident, const Vector2D& normal, float index )
	{
		float dot = Dot ( incident, normal );

		float square = 1.0F - index * index * ( 1.0F - dot * dot );

		if ( square < 0.0F )
		{
			return Reflect ( incident, normal );
		}
		else
		{
			return index * incident - ( sqrtf ( square ) + index * dot ) * normal;
		}
	}

	//----------------------------- Angle and Trigonometry Functions ------------------------------

	Vector2D Radians ( const Vector2D& source )
	{
		return Vector2D ( Radians ( source.X ), Radians ( source.Y ) );
	}

	Vector2D Degrees ( const Vector2D& source )
	{
		return Vector2D ( Degrees ( source.X ), Degrees ( source.Y ) );
	}

	Vector2D Sin ( const Vector2D& source )
	{
		return Vector2D ( sinf ( source.X ), sinf ( source.Y ) );
	}

	Vector2D Cos ( const Vector2D& source )
	{
		return Vector2D ( cosf ( source.X ), cosf ( source.Y ) );
	}

	Vector2D Tan ( const Vector2D& source )
	{
		return Vector2D ( tanf ( source.X ), tanf ( source.Y ) );
	}

	Vector2D Asin ( const Vector2D& source )
	{
		return Vector2D ( asinf ( source.X ), asinf ( source.Y ) );
	}

	Vector2D Acos ( const Vector2D& source )
	{
		return Vector2D ( acosf ( source.X ), acosf ( source.Y ) );
	}

	Vector2D Atan ( const Vector2D& source )
	{
		return Vector2D ( atanf ( source.X ), atanf ( source.Y ) );
	}

	//----------------------------------- Exponential Functions -----------------------------------

    Vector2D Pow ( const Vector2D& left, const Vector2D& right )
    {
		return Vector2D ( powf ( left.X, right.X ), powf ( left.Y, right.Y ) );
	}

    Vector2D Pow ( const Vector2D& left, float right )
    {
		return Vector2D ( powf ( left.X, right ), powf ( left.Y, right ) );
    }

    Vector2D Exp ( const Vector2D& source )
    {
		return Vector2D ( expf ( source.X ), expf ( source.Y ) );
    }

    Vector2D Log ( const Vector2D& source )
    {
		return Vector2D ( logf ( source.X ), logf ( source.Y ) );
    }

    Vector2D Sqrt ( const Vector2D& source )
    {
		return Vector2D ( sqrtf ( source.X ), sqrtf ( source.Y ) );
    }
}