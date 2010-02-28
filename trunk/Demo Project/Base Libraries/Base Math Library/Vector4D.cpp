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

#include "Vector4D.h"

namespace Math
{
	//-------------------------------------- Public Constants -------------------------------------

	const Vector4D Vector4D :: Zero ( 0.0F, 0.0F, 0.0F, 0.0F );

	const Vector4D Vector4D :: Unit ( 1.0F, 1.0F, 1.0F, 1.0F );

	const Vector4D Vector4D :: AxisX ( 1.0F, 0.0F, 0.0F, 0.0F );

	const Vector4D Vector4D :: AxisY ( 0.0F, 1.0F, 0.0F, 0.0F );

	const Vector4D Vector4D :: AxisZ ( 0.0F, 0.0F, 1.0F, 0.0F );

	const Vector4D Vector4D :: AxisW ( 0.0F, 0.0F, 0.0F, 1.0F );

	//--------------------------------------- Constructors ----------------------------------------

	Vector4D :: Vector4D ( float x, float y, float z, float w )
	{
		X = x;

		Y = y;

		Z = z;

		W = w;
	}

	Vector4D :: Vector4D ( const float vector [SIZE4D] )
	{
		X = vector [A];

		Y = vector [B];

		Z = vector [C];

		W = vector [D];
	}

	Vector4D :: Vector4D ( const Vector2D& vector, float z, float w )
	{
		X = vector [A];

		Y = vector [B];

		Z = z;

		W = w;
	}

	Vector4D :: Vector4D ( const Vector3D& vector, float w )
	{
		X = vector [A];

		Y = vector [B];

		Z = vector [C];
		
		W = w;
	}

	Vector4D :: Vector4D ( const Vector4D& vector )
	{
		X = vector [A];

		Y = vector [B];

		Z = vector [C];

		W = vector [D];
	}

	//------------------------------------ Arithmetic Operators -----------------------------------

	Vector4D operator - ( const Vector4D& source )
	{
		return Vector4D ( -source.X, -source.Y, -source.Z, -source.W );
	}

	Vector4D operator + ( const Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X + right.X, left.Y + right.Y,
			              left.Z + right.Z, left.W + right.W );
	}

	Vector4D operator - ( const Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X - right.X, left.Y - right.Y,
			              left.Z - right.Z, left.W - right.W );
	}

	Vector4D operator + ( const Vector4D& left, float right )
	{
		return Vector4D ( left.X + right, left.Y + right,
			              left.Z + right, left.W + right );
	}

	Vector4D operator - ( const Vector4D& left, float right )
	{
		return Vector4D ( left.X - right, left.Y - right,
			              left.Z - right, left.W - right );
	}

	Vector4D operator + ( float left, const Vector4D& right )
	{
		return Vector4D ( left + right.X, left + right.Y,
			              left + right.Z, left + right.W );
	}

	Vector4D operator - ( float left, const Vector4D& right )
	{
		return Vector4D ( left - right.X, left - right.Y,
			              left - right.Z, left - right.W );
	}

	Vector4D operator * ( const Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X * right.X, left.Y * right.Y,
			              left.Z * right.Z, left.W * right.W );
	}

	Vector4D operator / ( const Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X / right.X, left.Y / right.Y,
			              left.Z / right.Z, left.W / right.W );
	}

	Vector4D operator * ( const Vector4D& left, float right )
	{
		return Vector4D ( left.X * right, left.Y * right,
			              left.Z * right, left.W * right );
	}

	Vector4D operator / ( const Vector4D& left, float right )
	{
		return Vector4D ( left.X / right, left.Y / right,
			              left.Z / right, left.W / right );
	}

	Vector4D operator * ( float left, const Vector4D& right )
	{
		return Vector4D ( left * right.X, left * right.Y,
			              left * right.Z, left * right.W );
	}

	Vector4D operator / ( float left, const Vector4D& right )
	{
		return Vector4D ( left / right.X, left / right.Y,
			              left / right.Z, left / right.W );
	}

	Vector4D operator += ( Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X += right.X, left.Y += right.Y,
			              left.Z += right.Z, left.W += right.W );
	}

	Vector4D operator -= ( Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X -= right.X, left.Y -= right.Y,
			              left.Z -= right.Z, left.W -= right.W );
	}

	Vector4D operator += ( Vector4D& left, float right )
	{
		return Vector4D ( left.X += right, left.Y += right,
			              left.Z += right, left.W += right );
	}

	Vector4D operator -= ( Vector4D& left, float right )
	{
		return Vector4D ( left.X -= right, left.Y -= right,
			              left.Z -= right, left.W -= right );
	}

	Vector4D operator *= ( Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X *= right.X, left.Y *= right.Y,
			              left.Z *= right.Z, left.W *= right.W );
	}

	Vector4D operator /= ( Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X /= right.X, left.Y /= right.Y,
			              left.Z /= right.Z, left.W /= right.W );
	}

	Vector4D operator *= ( Vector4D& left, float right )
	{
		return Vector4D ( left.X *= right, left.Y *= right,
			              left.Z *= right, left.W *= right );
	}

	Vector4D operator /= ( Vector4D& left, float right )
	{
		return Vector4D ( left.X /= right, left.Y /= right,
			              left.Z /= right, left.W /= right );
	}

	//-------------------------------------- Logic Operators --------------------------------------

	bool operator < ( const Vector4D& left, const Vector4D& right )
	{
		return left.X < right.X && left.Y < right.Y && left.Z < right.Z && left.W < right.W;
	}

	bool operator > ( const Vector4D& left, const Vector4D& right )
	{
		return left.X > right.X && left.Y > right.Y && left.Z > right.Z && left.W > right.W;
	}

	bool operator < ( const Vector4D& left, float right )
	{
		return left.X < right && left.Y < right && left.Z < right && left.W < right;
	}

	bool operator > ( const Vector4D& left, float right )
	{
		return left.X > right && left.Y > right && left.Z > right && left.W > right;
	}

	bool operator < ( float left, const Vector4D& right )
	{
		return left < right.X && left < right.Y && left < right.Z && left < right.W;
	}

	bool operator > ( float left, const Vector4D& right )
	{
		return left > right.X && left > right.Y && left > right.Z && left > right.W;
	}

	bool operator <= ( const Vector4D& left, const Vector4D& right )
	{
		return left.X <= right.X && left.Y <= right.Y && left.Z <= right.Z && left.W <= right.W;
	}

	bool operator >= ( const Vector4D& left, const Vector4D& right )
	{
		return left.X >= right.X && left.Y >= right.Y && left.Z >= right.Z && left.W >= right.W;
	}

	bool operator <= ( const Vector4D& left, float right )
	{
		return left.X <= right && left.Y <= right && left.Z <= right && left.W <= right;
	}

	bool operator >= ( const Vector4D& left, float right )
	{
		return left.X >= right && left.Y >= right && left.Z >= right && left.W >= right;
	}

	bool operator <= ( float left, const Vector4D& right )
	{
		return left <= right.X && left <= right.Y && left <= right.Z && left <= right.W;
	}

	bool operator >= ( float left, const Vector4D& right )
	{
		return left >= right.X && left >= right.Y && left >= right.Z && left >= right.W;
	}

	bool operator == ( const Vector4D& left, const Vector4D& right )
	{
		return left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W;
	}

	bool operator != ( const Vector4D& left, const Vector4D& right )
	{
		return left.X != right.X || left.Y != right.Y || left.Z != right.Z || left.W != right.W;
	}

	bool operator == ( const Vector4D& left, float right )
	{
		return left.X == right && left.Y == right && left.Z == right && left.W == right;
	}

	bool operator != ( const Vector4D& left, float right )
	{
		return left.X != right || left.Y != right || left.Z != right || left.W != right;
	}

	bool operator == ( float left, const Vector4D& right )
	{
		return left == right.X && left == right.Y && left == right.Z && left == right.W;
	}

	bool operator != ( float left, const Vector4D& right )
	{
		return left != right.X || left != right.Y || left != right.Z || left != right.W;
	}

	//-------------------------------- Input and Output Operators ---------------------------------

	istream& operator >> ( istream& is, Vector4D& source )
	{
		return is >> source.X >> source.Y >> source.Z >> source.W;
	}

	ostream& operator << ( ostream& os, const Vector4D& source )
	{
		return os << source.X << " " << source.Y << " " << source.Z << " " << source.W;
	}

	//------------------------------------- Common Functions --------------------------------------
	Vector4D Abs ( const Vector4D& source )
	{
		return Vector4D ( fabsf ( source.X ), fabsf ( source.Y ),
			              fabsf ( source.Z ), fabsf ( source.W ) );
	}

	Vector4D Sign ( const Vector4D& source )
	{
		return Vector4D ( Sign ( source.X ), Sign ( source.Y ),
			              Sign ( source.Z ), Sign ( source.W ) );
	}

	Vector4D Floor ( const Vector4D& source )
	{
		return Vector4D ( floorf ( source.X ), floorf ( source.Y ),
			              floorf ( source.Z ), floorf ( source.W ) );
	}

	Vector4D Ceil ( const Vector4D& source )
	{
		return Vector4D ( ceilf ( source.X ), ceilf ( source.Y ),
			              ceilf ( source.Z ), ceilf ( source.W ) );
	}

	Vector4D Fract ( const Vector4D& source )
	{
		return Vector4D ( Fract ( source.X ), Fract ( source.Y ),
			              Fract ( source.Z ), Fract ( source.W ) );
	}

	Vector4D Mod ( const Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( Mod ( left.X, right.X ), Mod ( left.Y, right.Y ),
			              Mod ( left.Z, right.Z ), Mod ( left.W, right.W ) );
	}

	Vector4D Mod ( const Vector4D& left, float right )
	{
		return Vector4D ( Mod ( left.X, right ), Mod ( left.Y, right ),
			              Mod ( left.Z, right ), Mod ( left.W, right ) );
	}

	Vector4D Mod ( float left, const Vector4D& right )
	{
		return Vector4D ( Mod ( left, right.X ), Mod ( left, right.Y ),
			              Mod ( left, right.Z ), Mod ( left, right.W ) );
	}

	Vector4D Min ( const Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X < right.X ? left.X : right.X,
						  left.Y < right.Y ? left.Y : right.Y,
						  left.Z < right.Z ? left.Z : right.Z,
						  left.W < right.W ? left.W : right.W );
	}

	Vector4D Min ( const Vector4D& left, float right )
	{
		return Vector4D ( left.X < right ? left.X : right,
						  left.Y < right ? left.Y : right,
						  left.Z < right ? left.Z : right,
						  left.W < right ? left.W : right );
	}

	Vector4D Min ( float left, const Vector4D& right )
	{
		return Vector4D ( left < right.X ? left : right.X,
						  left < right.Y ? left : right.Y,
						  left < right.Z ? left : right.Z,
						  left < right.W ? left : right.W );
	}

	Vector4D Max ( const Vector4D& left, const Vector4D& right )
	{
		return Vector4D ( left.X > right.X ? left.X : right.X,
						  left.Y > right.Y ? left.Y : right.Y,
						  left.Z > right.Z ? left.Z : right.Z,
						  left.W > right.W ? left.W : right.W );
	}

	Vector4D Max ( const Vector4D& left, float right )
	{
		return Vector4D ( left.X > right ? left.X : right,
						  left.Y > right ? left.Y : right,
						  left.Z > right ? left.Z : right,
						  left.W > right ? left.W : right );
	}

	Vector4D Max ( float left, const Vector4D& right )
	{
		return Vector4D ( left > right.X ? left : right.X,
						  left > right.Y ? left : right.Y,
						  left > right.Z ? left : right.Z,
						  left > right.W ? left : right.W );
	}

	Vector4D Clamp ( const Vector4D& source, const Vector4D& min, const Vector4D& max )
	{
		return Vector4D ( Clamp ( source.X, min.X, max.X ), Clamp ( source.Y, min.Y, max.Y ),
						  Clamp ( source.Z, min.Z, max.Z ), Clamp ( source.W, min.W, max.W ) );
	}

	Vector4D Clamp ( const Vector4D& source, float min, float max )
	{
		return Vector4D ( Clamp ( source.X, min, max ), Clamp ( source.Y, min, max ),
						  Clamp ( source.Z, min, max ), Clamp ( source.W, min, max ) );
	}

	Vector4D Mix ( const Vector4D& left, const Vector4D& right, const Vector4D& alpha )
	{
		return Vector4D ( Mix ( left.X, right.X, alpha.X ), Mix ( left.Y, right.Y, alpha.Y ),
						  Mix ( left.Z, right.Z, alpha.Z ), Mix ( left.W, right.W, alpha.W ) );
	}

	Vector4D Mix ( const Vector4D& left, const Vector4D& right, float alpha )
	{
		return Vector4D ( Mix ( left.X, right.X, alpha ), Mix ( left.Y, right.Y, alpha ),
						  Mix ( left.Z, right.Z, alpha ), Mix ( left.W, right.W, alpha ) );
	}

	Vector4D Step ( const Vector4D& source, const Vector4D& value )
	{
		return Vector4D ( Step ( source.X, value.X ), Step ( source.Y, value.Y ),
						  Step ( source.Z, value.Z ), Step ( source.W, value.W ) );
	}

	Vector4D Step ( const Vector4D& source, float value )
	{
		return Vector4D ( Step ( source.X, value ), Step ( source.Y, value ),
						  Step ( source.Z, value ), Step ( source.W, value ) );
	}

	Vector4D Smooth ( const Vector4D& source, const Vector4D& min, const Vector4D& max )
	{
		return Vector4D ( Smooth ( source.X, min.X, max.X ), Smooth ( source.Y, min.Y, max.Y ),
						  Smooth ( source.Z, min.Z, max.Z ), Smooth ( source.W, min.W, max.W  ) );
	}

	Vector4D Smooth ( const Vector4D& source, float min, float max )
	{
		return Vector4D ( Smooth ( source.X, min, max ), Smooth ( source.Y, min, max ),
						  Smooth ( source.Z, min, max ), Smooth ( source.W, min, max ) );
	}

	//------------------------------------ Geometric Functions ------------------------------------

	float Length ( const Vector4D& source )
	{
		return sqrtf ( source.X * source.X + source.Y * source.Y +
			           source.Z * source.Z + source.W * source.W );
	}

	float Square ( const Vector4D& source )
	{
		return source.X * source.X + source.Y * source.Y +
			   source.Z * source.Z + source.W * source.W;
	}

	float Distance ( const Vector4D& left, const Vector4D& right )
	{
		return Length ( left - right );
	}

	float Dot ( const Vector4D& left, const Vector4D& right )
	{
		return left.X * right.X + left.Y * right.Y +
			   left.Z * right.Z + left.W * right.W;
	}        

	Vector4D Normalize ( const Vector4D& source )
	{
		return source / Length ( source );
	}

	Vector4D Reflect ( const Vector4D& incident, const Vector4D& normal )
	{
		return incident - 2.0F * Dot ( normal, incident ) * normal;
	}

	Vector4D Refract ( const Vector4D& incident, const Vector4D& normal, float index )
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

	Vector4D Radians ( const Vector4D& source )
	{
		return Vector4D ( Radians ( source.X ), Radians ( source.Y ),
						  Radians ( source.Z ), Radians ( source.W ) );
	}

	Vector4D Degrees ( const Vector4D& source )
	{
		return Vector4D ( Degrees ( source.X ), Degrees ( source.Y ),
						  Degrees ( source.Z ), Degrees ( source.W ) );
	}

	Vector4D Sin ( const Vector4D& source )
	{
		return Vector4D ( sinf ( source.X ), sinf ( source.Y ),
			              sinf ( source.Z ), sinf ( source.W ) );
	}

	Vector4D Cos ( const Vector4D& source )
	{
		return Vector4D ( cosf ( source.X ), cosf ( source.Y ),
			              cosf ( source.Z ), cosf ( source.W ) );
	}

	Vector4D Tan ( const Vector4D& source )
	{
		return Vector4D ( tanf ( source.X ), tanf ( source.Y ),
			              tanf ( source.Z ), tanf ( source.W ) );
	}

	Vector4D Asin ( const Vector4D& source )
	{
		return Vector4D ( asinf ( source.X ), asinf ( source.Y ),
			              asinf ( source.Z ), asinf ( source.W ) );
	}

	Vector4D Acos ( const Vector4D& source )
	{
		return Vector4D ( acosf ( source.X ), acosf ( source.Y ),
			              acosf ( source.Z ), acosf ( source.W ) );
	}

	Vector4D Atan ( const Vector4D& source )
	{
		return Vector4D ( atanf ( source.X ), atanf ( source.Y ),
			              atanf ( source.Z ), atanf ( source.W ) );
	}

	//----------------------------------- Exponential Functions -----------------------------------

    Vector4D Pow ( const Vector4D& left, const Vector4D& right )
    {
		return Vector4D ( powf ( left.X, right.X ), powf ( left.Y, right.Y ),
						  powf ( left.Z, right.Z ), powf ( left.W, right.W ) );
	}

    Vector4D Pow ( const Vector4D& left, float right )
    {
		return Vector4D ( powf ( left.X, right ), powf ( left.Y, right ),
						  powf ( left.Z, right ), powf ( left.W, right ) );
    }

    Vector4D Exp ( const Vector4D& source )
    {
		return Vector4D ( expf ( source.X ), expf ( source.Y ),
						  expf ( source.Z ), expf ( source.W ) );
    }

    Vector4D Log ( const Vector4D& source )
    {
		return Vector4D ( logf ( source.X ), logf ( source.Y ),
						  logf ( source.Z ), logf ( source.W ) );
    }

    Vector4D Sqrt ( const Vector4D& source )
    {
		return Vector4D ( sqrtf ( source.X ), sqrtf ( source.Y ),
						  sqrtf ( source.Z ), sqrtf ( source.W ) );
    }
}