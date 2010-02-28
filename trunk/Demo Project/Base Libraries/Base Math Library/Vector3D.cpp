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

#include "Vector3D.h"

namespace Math
{
	//-------------------------------------- Public Constants -------------------------------------

	const Vector3D Vector3D :: Zero ( 0.0F, 0.0F, 0.0F );

	const Vector3D Vector3D :: Unit ( 1.0F, 1.0F, 1.0F );

	const Vector3D Vector3D :: AxisX ( 1.0F, 0.0F, 0.0F );

	const Vector3D Vector3D :: AxisY ( 0.0F, 1.0F, 0.0F );

	const Vector3D Vector3D :: AxisZ ( 0.0F, 0.0F, 1.0F );

	//--------------------------------------- Constructors ----------------------------------------

	Vector3D :: Vector3D ( float x, float y, float z )
	{
		X = x;

		Y = y;

		Z = z;
	}

	Vector3D :: Vector3D ( const float vector [SIZE3D] )
	{
		X = vector [A];

		Y = vector [B];

		Z = vector [C];
	}

	Vector3D :: Vector3D ( const Vector2D& vector, float z )
	{
		X = vector [A];

		Y = vector [B];

		Z = z;
	}

	Vector3D :: Vector3D ( const Vector3D& vector )
	{
		X = vector [A];

		Y = vector [B];

		Z = vector [C];
	}

	Vector3D :: Vector3D ( const Vector4D& vector )
	{
		X = vector [A];

		Y = vector [B];

		Z = vector [C];
	}

	//------------------------------------ Arithmetic Operators -----------------------------------

	Vector3D operator - ( const Vector3D& source )
	{
		return Vector3D ( -source.X, -source.Y, -source.Z );
	}

	Vector3D operator + ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X + right.X, left.Y + right.Y, left.Z + right.Z );
	}

	Vector3D operator - ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X - right.X, left.Y - right.Y, left.Z - right.Z );
	}

	Vector3D operator + ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X + right, left.Y + right, left.Z + right );
	}

	Vector3D operator - ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X - right, left.Y - right, left.Z - right );
	}

	Vector3D operator + ( float left, const Vector3D& right )
	{
		return Vector3D ( left + right.X, left + right.Y, left + right.Z );
	}

	Vector3D operator - ( float left, const Vector3D& right )
	{
		return Vector3D ( left - right.X, left - right.Y, left - right.Z );
	}

	Vector3D operator * ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X * right.X, left.Y * right.Y, left.Z * right.Z );
	}

	Vector3D operator / ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X / right.X, left.Y / right.Y, left.Z / right.Z );
	}

	Vector3D operator * ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X * right, left.Y * right, left.Z * right );
	}

	Vector3D operator / ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X / right, left.Y / right, left.Z / right );
	}

	Vector3D operator * ( float left, const Vector3D& right )
	{
		return Vector3D ( left * right.X, left * right.Y, left * right.Z );
	}

	Vector3D operator / ( float left, const Vector3D& right )
	{
		return Vector3D ( left / right.X, left / right.Y, left / right.Z );
	}

	Vector3D operator += ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X += right.X, left.Y += right.Y, left.Z += right.Z );
	}

	Vector3D operator -= ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X -= right.X, left.Y -= right.Y, left.Z -= right.Z );
	}

	Vector3D operator += ( Vector3D& left, float right )
	{
		return Vector3D ( left.X += right, left.Y += right, left.Z += right );
	}

	Vector3D operator -= ( Vector3D& left, float right )
	{
		return Vector3D ( left.X -= right, left.Y -= right, left.Z -= right );
	}

	Vector3D operator *= ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X *= right.X, left.Y *= right.Y, left.Z *= right.Z );
	}

	Vector3D operator /= ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X /= right.X, left.Y /= right.Y, left.Z /= right.Z );
	}

	Vector3D operator *= ( Vector3D& left, float right )
	{
		return Vector3D ( left.X *= right, left.Y *= right, left.Z *= right );
	}

	Vector3D operator /= ( Vector3D& left, float right )
	{
		return Vector3D ( left.X /= right, left.Y /= right, left.Z /= right );
	}

	//-------------------------------------- Logic Operators --------------------------------------

	bool operator < ( const Vector3D& left, const Vector3D& right )
	{
		return left.X < right.X && left.Y < right.Y && left.Z < right.Z;
	}

	bool operator > ( const Vector3D& left, const Vector3D& right )
	{
		return left.X > right.X && left.Y > right.Y && left.Z > right.Z;
	}

	bool operator < ( const Vector3D& left, float right )
	{
		return left.X < right && left.Y < right && left.Z < right;
	}

	bool operator > ( const Vector3D& left, float right )
	{
		return left.X > right && left.Y > right && left.Z > right;
	}

	bool operator < ( float left, const Vector3D& right )
	{
		return left < right.X && left < right.Y && left < right.Z;
	}

	bool operator > ( float left, const Vector3D& right )
	{
		return left > right.X && left > right.Y && left > right.Z;
	}

	bool operator <= ( const Vector3D& left, const Vector3D& right )
	{
		return left.X <= right.X && left.Y <= right.Y && left.Z <= right.Z;
	}

	bool operator >= ( const Vector3D& left, const Vector3D& right )
	{
		return left.X >= right.X && left.Y >= right.Y && left.Z >= right.Z;
	}

	bool operator <= ( const Vector3D& left, float right )
	{
		return left.X <= right && left.Y <= right && left.Z <= right;
	}

	bool operator >= ( const Vector3D& left, float right )
	{
		return left.X >= right && left.Y >= right && left.Z >= right;
	}

	bool operator <= ( float left, const Vector3D& right )
	{
		return left <= right.X && left <= right.Y && left <= right.Z;
	}

	bool operator >= ( float left, const Vector3D& right )
	{
		return left >= right.X && left >= right.Y && left >= right.Z;
	}

	bool operator == ( const Vector3D& left, const Vector3D& right )
	{
		return left.X == right.X && left.Y == right.Y && left.Z == right.Z;
	}

	bool operator != ( const Vector3D& left, const Vector3D& right )
	{
		return left.X != right.X || left.Y != right.Y || left.Z != right.Z;
	}

	bool operator == ( const Vector3D& left, float right )
	{
		return left.X == right && left.Y == right && left.Z == right;
	}

	bool operator != ( const Vector3D& left, float right )
	{
		return left.X != right || left.Y != right || left.Z != right;
	}

	bool operator == ( float left, const Vector3D& right )
	{
		return left == right.X && left == right.Y && left == right.Z;
	}

	bool operator != ( float left, const Vector3D& right )
	{
		return left != right.X || left != right.Y || left != right.Z;
	}
	
	//-------------------------------- Input and Output Operators ---------------------------------

	istream& operator >> ( istream& is, Vector3D& source )
	{
		return is >> source.X >> source.Y >> source.Z;
	}

	ostream& operator << ( ostream& os, const Vector3D& source )
	{
		return os << source.X << " " << source.Y << " " << source.Z;
	}

	//------------------------------------- Common Functions --------------------------------------

	Vector3D Abs ( const Vector3D& source )
	{
		return Vector3D ( fabsf ( source.X ), fabsf ( source.Y ), fabsf ( source.Z ) );
	}

	Vector3D Sign ( const Vector3D& source )
	{
		return Vector3D ( Sign ( source.X ), Sign ( source.Y ), Sign ( source.Z ) );
	}

	Vector3D Floor ( const Vector3D& source )
	{
		return Vector3D ( floorf ( source.X ), floorf ( source.Y ), floorf ( source.Z ) );
	}

	Vector3D Ceil ( const Vector3D& source )
	{
		return Vector3D ( ceilf ( source.X ), ceilf ( source.Y ), ceilf ( source.Z ) );
	}

	Vector3D Fract ( const Vector3D& source )
	{
		return Vector3D ( Fract ( source.X ), Fract ( source.Y ), Fract ( source.Z ) );
	}

	Vector3D Mod ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( Mod ( left.X, right.X ), Mod ( left.Y, right.Y ), Mod ( left.Z, right.Z ) );
	}

	Vector3D Mod ( const Vector3D& left, float right )
	{
		return Vector3D ( Mod ( left.X, right ), Mod ( left.Y, right ), Mod ( left.Z, right ) );
	}

	Vector3D Mod ( float left, const Vector3D& right )
	{
		return Vector3D ( Mod ( left, right.X ), Mod ( left, right.Y ), Mod ( left, right.Z ) );
	}

	Vector3D Min ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X < right.X ? left.X : right.X,
						  left.Y < right.Y ? left.Y : right.Y,
						  left.Z < right.Z ? left.Z : right.Z );
	}

	Vector3D Min ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X < right ? left.X : right,
						  left.Y < right ? left.Y : right,
						  left.Z < right ? left.Z : right );
	}

	Vector3D Min ( float left, const Vector3D& right )
	{
		return Vector3D ( left < right.X ? left : right.X,
						  left < right.Y ? left : right.Y,
						  left < right.Z ? left : right.Z );
	}

	Vector3D Max ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X > right.X ? left.X : right.X,
						  left.Y > right.Y ? left.Y : right.Y,
						  left.Z > right.Z ? left.Z : right.Z );
	}

	Vector3D Max ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X > right ? left.X : right,
						  left.Y > right ? left.Y : right,
						  left.Z > right ? left.Z : right );
	}

	Vector3D Max ( float left, const Vector3D& right )
	{
		return Vector3D ( left > right.X ? left : right.X,
						  left > right.Y ? left : right.Y,
						  left > right.Z ? left : right.Z );
	}

	Vector3D Clamp ( const Vector3D& source, const Vector3D& min, const Vector3D& max )
	{
		return Vector3D ( Clamp ( source.X, min.X, max.X ),
			              Clamp ( source.Y, min.Y, max.Y ),
						  Clamp ( source.Z, min.Z, max.Z ) );
	}

	Vector3D Clamp ( const Vector3D& source, float min, float max )
	{
		return Vector3D ( Clamp ( source.X, min, max ),
			              Clamp ( source.Y, min, max ),
						  Clamp ( source.Z, min, max ) );
	}

	Vector3D Mix ( const Vector3D& left, const Vector3D& right, const Vector3D& alpha )
	{
		return Vector3D ( Mix ( left.X, right.X, alpha.X ),
			              Mix ( left.Y, right.Y, alpha.Y ),
						  Mix ( left.Z, right.Z, alpha.Z ) );
	}

	Vector3D Mix ( const Vector3D& left, const Vector3D& right, float alpha )
	{
		return Vector3D ( Mix ( left.X, right.X, alpha ),
			              Mix ( left.Y, right.Y, alpha ),
						  Mix ( left.Z, right.Z, alpha ) );
	}

	Vector3D Step ( const Vector3D& source, const Vector3D& value )
	{
		return Vector3D ( Step ( source.X, value.X ),
			              Step ( source.Y, value.Y ),
						  Step ( source.Z, value.Z ) );
	}

	Vector3D Step ( const Vector3D& source, float value )
	{
		return Vector3D ( Step ( source.X, value ),
			              Step ( source.Y, value ),
						  Step ( source.Z, value ) );
	}

	Vector3D Smooth ( const Vector3D& source, const Vector3D& min, const Vector3D& max )
	{
		return Vector3D ( Smooth ( source.X, min.X, max.X ),
			              Smooth ( source.Y, min.Y, max.Y ),
						  Smooth ( source.Z, min.Z, max.Z ) );
	}

	Vector3D Smooth ( const Vector3D& source, float min, float max )
	{
		return Vector3D ( Smooth ( source.X, min, max ),
			              Smooth ( source.Y, min, max ),
						  Smooth ( source.Z, min, max ) );
	}

	//------------------------------------ Geometric Functions ------------------------------------

	float Length ( const Vector3D& source )
	{
		return sqrtf ( source.X * source.X + source.Y * source.Y + source.Z * source.Z );
	}

	float Square ( const Vector3D& source )
	{
		return source.X * source.X + source.Y * source.Y + source.Z * source.Z;
	}

	float Distance ( const Vector3D& left, const Vector3D& right )
	{
		return Length ( left - right );
	}

	float Dot ( const Vector3D& left, const Vector3D& right )
	{
		return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
	}
	
	Vector3D Cross ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.Y * right.Z - left.Z * right.Y,
			              left.Z * right.X - left.X * right.Z,
						  left.X * right.Y - left.Y * right.X );
	} 

	Vector3D Normalize ( const Vector3D& source )
	{
		return source / Length ( source );
	}

	Vector3D Reflect ( const Vector3D& incident, const Vector3D& normal )
	{
		return incident - 2.0F * Dot ( normal, incident ) * normal;
	}

	Vector3D Refract ( const Vector3D& incident, const Vector3D& normal, float index )
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

	Vector3D Radians ( const Vector3D& source )
	{
		return Vector3D ( Radians ( source.X ),
			              Radians ( source.Y ),
						  Radians ( source.Z ) );
	}

	Vector3D Degrees ( const Vector3D& source )
	{
		return Vector3D ( Degrees ( source.X ),
			              Degrees ( source.Y ),
						  Degrees ( source.Z ) );
	}

	Vector3D Sin ( const Vector3D& source )
	{
		return Vector3D ( sinf ( source.X ), sinf ( source.Y ), sinf ( source.Z ) );
	}

	Vector3D Cos ( const Vector3D& source )
	{
		return Vector3D ( cosf ( source.X ), cosf ( source.Y ), cosf ( source.Z ) );
	}

	Vector3D Tan ( const Vector3D& source )
	{
		return Vector3D ( tanf ( source.X ), tanf ( source.Y ), tanf ( source.Z ) );
	}

	Vector3D Asin ( const Vector3D& source )
	{
		return Vector3D ( asinf ( source.X ), asinf ( source.Y ), asinf ( source.Z ) );
	}

	Vector3D Acos ( const Vector3D& source )
	{
		return Vector3D ( acosf ( source.X ), acosf ( source.Y ), acosf ( source.Z ) );
	}

	Vector3D Atan ( const Vector3D& source )
	{
		return Vector3D ( atanf ( source.X ), atanf ( source.Y ), atanf ( source.Z ) );
	}

	//----------------------------------- Exponential Functions -----------------------------------

    Vector3D Pow ( const Vector3D& left, const Vector3D& right )
    {
		return Vector3D ( powf ( left.X, right.X ),
			              powf ( left.Y, right.Y ),
						  powf ( left.Z, right.Z ) );
	}

    Vector3D Pow ( const Vector3D& left, float right )
    {
		return Vector3D ( powf ( left.X, right ),
			              powf ( left.Y, right ),
						  powf ( left.Z, right ) );
    }

    Vector3D Exp ( const Vector3D& source )
    {
		return Vector3D ( expf ( source.X ), expf ( source.Y ), expf ( source.Z ) );
    }

    Vector3D Log ( const Vector3D& source )
    {
		return Vector3D ( logf ( source.X ), logf ( source.Y ), logf ( source.Z ) );
    }

    Vector3D Sqrt ( const Vector3D& source )
    {
		return Vector3D ( sqrtf ( source.X ), sqrtf ( source.Y ), sqrtf ( source.Z ) );
    }
}