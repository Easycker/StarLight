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

#include "Matrix3D.h"

#include <math.h>

namespace Math
{
	//------------------------------------- Public Constants --------------------------------------

	const Matrix3D Matrix3D :: Zero ( 0.0F );

	const Matrix3D Matrix3D :: Unit ( 1.0F );

	//--------------------------------------- Constructors ----------------------------------------
	
	Matrix3D :: Matrix3D ( float diagonal )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			values [i][i] = diagonal;
		}
	}

	Matrix3D :: Matrix3D ( const float diagonal [SIZE3D] )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			values [i][i] = diagonal [i];
		}
	}
	
	Matrix3D :: Matrix3D ( const Vector3D& diagonal )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			values [i][i] = diagonal [i];
		}
	}
	
	Matrix3D :: Matrix3D ( const float matrix [SIZE3D][SIZE3D] )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				values [i][j] = matrix [i][j];
			}
		}
	}
	
	Matrix3D :: Matrix3D ( const Matrix3D& matrix )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				values [i][j] = matrix [i][j];
			}
		}
	}

	//------------------------------------ Arithmetic Operators -----------------------------------

	Matrix3D operator - ( const Matrix3D& source )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = -source [i][j];
			}
		}

		return result;
	}

	Matrix3D operator + ( const Matrix3D& left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = left [i][j] + right [i][j];
			}
		}

		return result;
	}

	Matrix3D operator - ( const Matrix3D& left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = left [i][j] - right [i][j];
			}
		}

		return result;
	}

	Matrix3D operator * ( const Matrix3D& left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				for ( int k = 0; k < SIZE3D; k++ )
				{
					result [i][j] += left [i][k] * right [k][j];
				}
			}
		}

		return result;
	}
			
	Vector3D operator * ( const Matrix3D& left, const Vector3D& right )
	{
		Vector3D result = Vector3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int k = 0; k < SIZE3D; k++ )
			{
				result [i] += left [i][k] * right [k];
			}
		}
		
		return result;
	}

	Matrix3D operator * ( const Matrix3D& left, float right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = left [i][j] * right;
			}
		}

		return result;
	}

	Matrix3D operator / ( const Matrix3D& left, float right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = left [i][j] / right;
			}
		}

		return result;
	}
			
	Matrix3D operator * ( float left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = left * right [i][j];
			}
		}

		return result;
	}

	Matrix3D operator / ( float left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = left / right [i][j];
			}
		}

		return result;
	}
	
	//-------------------------------- Input and Output Operators ---------------------------------
	
	istream& operator >> ( istream& is, Matrix3D& source )
	{
		for ( int j = 0; j < SIZE3D; j++ )
		{
			for ( int i = 0; i < SIZE3D; i++ )
			{
				is >> source [i][j];
			}
		}
		return is;
	}	
	
	ostream& operator << ( ostream& os, const Matrix3D& source )
	{
		for ( int j = 0; j < SIZE3D; j++ )
		{
			for ( int i = 0; i < SIZE3D; i++ )
			{
				os << source [i][j] << " ";
			}

			os << endl;
		}
		return os;
	}

	//------------------------------------- Common Functions --------------------------------------

	float Determinant ( const Matrix3D& source )
	{
		return source [A][A] * source [B][B] * source [C][C] +
			   source [B][A] * source [C][B] * source [A][C] +
			   source [C][A] * source [A][B] * source [B][C] -
			   source [A][A] * source [C][B] * source [B][C] -
			   source [C][A] * source [B][B] * source [A][C] -
			   source [B][A] * source [A][B] * source [C][C];
	}

	Matrix3D Transpose ( const Matrix3D& source )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result [i][j] = source [j][i];
			}
		}

		return result;
	} 

	Matrix3D Adjugate ( const Matrix3D& source )
	{
		Matrix3D result = Matrix3D :: Zero;
		
		result [A][A] = source [B][B] * source [C][C] - source [B][C] * source [C][B];

		result [A][B] = source [A][C] * source [C][B] - source [A][B] * source [C][C];

		result [A][C] = source [A][B] * source [B][C] - source [A][C] * source [B][B];

		result [B][A] = source [B][C] * source [C][A] - source [B][A] * source [C][C];

		result [B][B] = source [A][A] * source [C][C] - source [A][C] * source [C][A];

		result [B][C] = source [A][C] * source [B][A] - source [A][A] * source [B][C];

		result [C][A] = source [B][A] * source [C][B] - source [B][B] * source [C][A];

		result [C][B] = source [A][B] * source [C][A] - source [A][A] * source [C][B];

		result [C][C] = source [A][A] * source [B][B] - source [A][B] * source [B][A];
		
		return result;
	}

	Matrix3D Inverse ( const Matrix3D& source )
	{
		return Adjugate ( source ) / Determinant ( source );
	}

	//------------------------------------- Specific Functions ------------------------------------

	Matrix3D Matrix3D :: MirrorX ( void )
	{
		Matrix3D result = Matrix3D :: Unit;

		result [A][A] = -1.0F;		

		return result;
	}
			
	Matrix3D Matrix3D :: MirrorY ( void )
	{
		Matrix3D result = Matrix3D :: Unit;

		result [B][B] = -1.0F;

		return result;
	}
	
	Matrix3D Matrix3D :: MirrorZ ( void )
	{
		Matrix3D result = Matrix3D :: Unit;

		result [C][C] = -1.0F;
		
		return result;
	}

	Matrix3D Matrix3D :: RotateX ( float angle )
	{
		Matrix3D result = Matrix3D :: Unit;
		
		float cos = cosf ( angle );
		
		float sin = sinf ( angle );

		result [B][B] = cos;
		
		result [B][C] = sin;

		result [C][B] = -sin;
		
		result [C][C] = cos;

		return result;
	}
			
	Matrix3D Matrix3D :: RotateY ( float angle )
	{
		Matrix3D result = Matrix3D :: Unit;
		
		float cos = cosf ( angle );
		
		float sin = sinf ( angle );

		result [A][A] = cos;
		
		result [A][C] = -sin;
		
		result [C][A] = sin;
		
		result [C][C] = cos;

		return result;
	}
	
	Matrix3D Matrix3D :: RotateZ ( float angle )
	{
		Matrix3D result = Matrix3D :: Unit;
		
		float cos = cosf ( angle );
		
		float sin = sinf ( angle );

		result [A][A] = cos;
		
		result [A][B] = sin;
		
		result [B][A] = -sin;
		
		result [B][B] = cos;
		
		return result;
	}

	Matrix3D Matrix3D :: Rotate ( float angle, const Vector3D& direction )
	{
		Matrix3D result = Matrix3D :: Unit;
		
		float cos = cosf ( angle );
		
		float sin = sinf ( angle );

		result [A][A] = direction.X * direction.X + ( 1.0F - direction.X * direction.X) * cos;

		result [A][B] = direction.X * direction.Y * ( 1.0F - cos ) + direction.Z * sin;

		result [A][C] = direction.X * direction.Z * ( 1.0F - cos ) - direction.Y * sin;

		result [B][A] = direction.X * direction.Y * ( 1.0F - cos ) - direction.Z * sin;

		result [B][B] = direction.Y * direction.Y + ( 1.0F - direction.Y * direction.Y ) * cos;

		result [B][C] = direction.Y * direction.Z * ( 1.0F - cos ) + direction.X * sin;

		result [C][A] = direction.X * direction.Z * ( 1.0F - cos ) + direction.Y * sin;

		result [C][B] = direction.Y * direction.Z * ( 1.0F - cos ) - direction.X * sin;

		result [C][C] = direction.Z * direction.Z + ( 1.0F - direction.Z * direction.Z ) * cos;

		return result;
	}

	Matrix3D Matrix3D :: Rotate ( const Vector3D& orientation )
	{
		return RotateZ ( orientation.Z ) * RotateY ( orientation.Y ) * RotateX ( orientation.X );
	}

	Matrix3D Matrix3D :: ScaleX ( float scale )
	{
		Matrix3D result = Matrix3D :: Unit;

		result [A][A] = scale;		

		return result;
	}
			
	Matrix3D Matrix3D :: ScaleY ( float scale )
	{
		Matrix3D result = Matrix3D :: Unit;

		result [B][B] = scale;

		return result;
	}
	
	Matrix3D Matrix3D :: ScaleZ ( float scale )
	{
		Matrix3D result = Matrix3D :: Unit;

		result [C][C] = scale;
		
		return result;
	}

	Matrix3D Matrix3D :: Scale ( const Vector3D& scaling )
	{
		Matrix3D result = Matrix3D :: Unit;

		result [A][A] = scaling.X;
		
		result [B][B] = scaling.Y;
		
		result [C][C] = scaling.Z;
		
		return result;
	}
}