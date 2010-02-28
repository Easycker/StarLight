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

#include "Matrix4D.h"

namespace Math
{
	//------------------------------------- Public Constants --------------------------------------

	const Matrix4D Matrix4D :: Zero ( 0.0F );

	const Matrix4D Matrix4D :: Unit ( 1.0F );

	//--------------------------------------- Constructors ----------------------------------------
	
	Matrix4D :: Matrix4D ( float diagonal )
	{
		for ( int i = 0; i < SIZE4D; i++ )
		{
			values [i][i] = diagonal;
		}
	}

	Matrix4D :: Matrix4D ( const float diagonal [SIZE4D] )
	{
		for ( int i = 0; i < SIZE4D; i++ )
		{
			values [i][i] = diagonal [i];
		}
	}
	
	Matrix4D :: Matrix4D ( const Vector4D& diagonal )
	{
		for ( int i = 0; i < SIZE4D; i++ )
		{
			values [i][i] = diagonal [i];
		}
	}
	
	Matrix4D :: Matrix4D ( const float matrix [SIZE4D][SIZE4D] )
	{
		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				values [i][j] = matrix [i][j];
			}
		}
	}
	
	Matrix4D :: Matrix4D ( const Matrix4D& matrix )
	{
		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				values [i][j] = matrix [i][j];
			}
		}
	}

	//------------------------------------ Arithmetic Operators -----------------------------------

	Matrix4D operator - ( const Matrix4D& source )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = -source [i][j];
			}
		}

		return result;
	}

	Matrix4D operator + ( const Matrix4D& left, const Matrix4D& right )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = left [i][j] + right [i][j];
			}
		}

		return result;
	}

	Matrix4D operator - ( const Matrix4D& left, const Matrix4D& right )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = left [i][j] - right [i][j];
			}
		}

		return result;
	}

	Matrix4D operator * ( const Matrix4D& left, const Matrix4D& right )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				for ( int k = 0; k < SIZE4D; k++ )
				{
					result [i][j] += left [i][k] * right [k][j];
				}
			}
		}

		return result;
	}
			
	Vector4D operator * ( const Matrix4D& left, const Vector4D& right )
	{
		Vector4D result = Vector4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int k = 0; k < SIZE4D; k++ )
			{
				result [i] += left [i][k] * right[k];
			}
		}
		
		return result;
	}

	Matrix4D operator * ( const Matrix4D& left, float right )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = left [i][j] * right;
			}
		}

		return result;
	}

	Matrix4D operator / ( const Matrix4D& left, float right )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = left [i][j] / right;
			}
		}

		return result;
	}
			
	Matrix4D operator * ( float left, const Matrix4D& right )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = left * right [i][j];
			}
		}

		return result;
	}

	Matrix4D operator / ( float left, const Matrix4D& right )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = left / right [i][j];
			}
		}

		return result;
	}
	
	//-------------------------------- Input and Output Operators ---------------------------------
	
	istream& operator >> ( istream& is, Matrix4D& source )
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
	
	ostream& operator << ( ostream& os, const Matrix4D& source )
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

	float Determinant ( const Matrix4D& source )
	{
		return source [A][A] * source [B][B] * source [C][C] * source [D][D] - 
			   source [A][A] * source [B][B] * source [C][D] * source [D][C] + 
			   source [A][A] * source [B][C] * source [C][D] * source [D][B] - 
			   source [A][A] * source [B][C] * source [C][B] * source [D][D] + 
			   source [A][A] * source [B][D] * source [C][B] * source [D][C] - 
			   source [A][A] * source [B][D] * source [C][C] * source [D][B] - 
			   source [A][B] * source [B][C] * source [C][D] * source [D][A] + 
			   source [A][B] * source [B][C] * source [C][A] * source [D][D] - 
			   source [A][B] * source [B][D] * source [C][A] * source [D][C] + 
			   source [A][B] * source [B][D] * source [C][C] * source [D][A] - 
			   source [A][B] * source [B][A] * source [C][C] * source [D][D] + 
			   source [A][B] * source [B][A] * source [C][D] * source [D][C] + 
			   source [A][C] * source [B][D] * source [C][A] * source [D][B] - 
			   source [A][C] * source [B][D] * source [C][B] * source [D][A] + 
			   source [A][C] * source [B][A] * source [C][B] * source [D][D] - 
			   source [A][C] * source [B][A] * source [C][D] * source [D][B] + 
			   source [A][C] * source [B][B] * source [C][D] * source [D][A] - 
			   source [A][C] * source [B][B] * source [C][A] * source [D][D] - 
			   source [A][D] * source [B][A] * source [C][B] * source [D][C] + 
			   source [A][D] * source [B][A] * source [C][C] * source [D][B] -
			   source [A][D] * source [B][B] * source [C][C] * source [D][A] + 
			   source [A][D] * source [B][B] * source [C][A] * source [D][C] - 
			   source [A][D] * source [B][C] * source [C][A] * source [D][B] + 
			   source [A][D] * source [B][C] * source [C][B] * source [D][A];
	}

	Matrix4D Transpose ( const Matrix4D& source )
	{
		Matrix4D result = Matrix4D :: Zero;

		for ( int i = 0; i < SIZE4D; i++ )
		{
			for ( int j = 0; j < SIZE4D; j++ )
			{
				result [i][j] = source [j][i];
			}
		}

		return result;
	} 

	Matrix4D Adjugate ( const Matrix4D& source )
	{
		Matrix4D result = Matrix4D :: Zero;
		
		result [A][A] = source [B][C] * source [C][D] * source [D][B] -
			            source [B][D] * source [C][C] * source [D][B] +
					    source [B][D] * source [C][B] * source [D][C] -
					    source [B][B] * source [C][D] * source [D][C] -
					    source [B][C] * source [C][B] * source [D][D] +
					    source [B][B] * source [C][C] * source [D][D];

		result [A][B] = source [A][D] * source [C][C] * source [D][B] -
			            source [A][C] * source [C][D] * source [D][B] -
					    source [A][D] * source [C][B] * source [D][C] +
					    source [A][B] * source [C][D] * source [D][C] +
					    source [A][C] * source [C][B] * source [D][D] -
					    source [A][B] * source [C][C] * source [D][D];

		result [A][C] = source [A][C] * source [B][D] * source [D][B] -
			            source [A][D] * source [B][C] * source [D][B] +
					    source [A][D] * source [B][B] * source [D][C] -
					    source [A][B] * source [B][D] * source [D][C] -
					    source [A][C] * source [B][B] * source [D][D] +
					    source [A][B] * source [B][C] * source [D][D];

		result [A][D] = source [A][D] * source [B][C] * source [C][B] -
			            source [A][C] * source [B][D] * source [C][B] -
					    source [A][D] * source [B][B] * source [C][C] +
					    source [A][B] * source [B][D] * source [C][C] +
					    source [A][C] * source [B][B] * source [C][D] -
					    source [A][B] * source [B][C] * source [C][D];

		result [B][A] = source [B][D] * source [C][C] * source [D][A] -
			            source [B][C] * source [C][D] * source [D][A] -
					    source [B][D] * source [C][A] * source [D][C] +
					    source [B][A] * source [C][D] * source [D][C] +
					    source [B][C] * source [C][A] * source [D][D] -
					    source [B][A] * source [C][C] * source [D][D];

		result [B][B] = source [A][C] * source [C][D] * source [D][A] -
			            source [A][D] * source [C][C] * source [D][A] +
					    source [A][D] * source [C][A] * source [D][C] -
					    source [A][A] * source [C][D] * source [D][C] -
					    source [A][C] * source [C][A] * source [D][D] +
					    source [A][A] * source [C][C] * source [D][D];

		result [B][C] = source [A][D] * source [B][C] * source [D][A] -
			            source [A][C] * source [B][D] * source [D][A] -
					    source [A][D] * source [B][A] * source [D][C] +
					    source [A][A] * source [B][D] * source [D][C] +
					    source [A][C] * source [B][A] * source [D][D] -
					    source [A][A] * source [B][C] * source [D][D];

		result [B][D] = source [A][C] * source [B][D] * source [C][A] -
			            source [A][D] * source [B][C] * source [C][A] +
					    source [A][D] * source [B][A] * source [C][C] -
					    source [A][A] * source [B][D] * source [C][C] -
					    source [A][C] * source [B][A] * source [C][D] +
					    source [A][A] * source [B][C] * source [C][D];

		result [C][A] = source [B][B] * source [C][D] * source [D][A] -
			            source [B][D] * source [C][B] * source [D][A] +
					    source [B][D] * source [C][A] * source [D][B] -
					    source [B][A] * source [C][D] * source [D][B] -
					    source [B][B] * source [C][A] * source [D][D] +
					    source [B][A] * source [C][B] * source [D][D];

		result [C][B] = source [A][D] * source [C][B] * source [D][A] -
			            source [A][B] * source [C][D] * source [D][A] -
					    source [A][D] * source [C][A] * source [D][B] +
					    source [A][A] * source [C][D] * source [D][B] +
					    source [A][B] * source [C][A] * source [D][D] -
					    source [A][A] * source [C][B] * source [D][D];

		result [C][C] = source [A][B] * source [B][D] * source [D][A] -
			            source [A][D] * source [B][B] * source [D][A] +
					    source [A][D] * source [B][A] * source [D][B] -
				 	    source [A][A] * source [B][D] * source [D][B] -
					    source [A][B] * source [B][A] * source [D][D] +
					    source [A][A] * source [B][B] * source [D][D];

		result [C][D] = source [A][D] * source [B][B] * source [C][A] -
			            source [A][B] * source [B][D] * source [C][A] -
				  	    source [A][D] * source [B][A] * source [C][B] +
				 	    source [A][A] * source [B][D] * source [C][B] +
				 	    source [A][B] * source [B][A] * source [C][D] -
				 	    source [A][A] * source [B][B] * source [C][D];

		result [D][A] = source [B][C] * source [C][B] * source [D][A] -
			            source [B][B] * source [C][C] * source [D][A] -
					    source [B][C] * source [C][A] * source [D][B] +
					    source [B][A] * source [C][C] * source [D][B] +
					    source [B][B] * source [C][A] * source [D][C] -
					    source [B][A] * source [C][B] * source [D][C];

		result [D][B] = source [A][B] * source [C][C] * source [D][A] -
			            source [A][C] * source [C][B] * source [D][A] +
					    source [A][C] * source [C][A] * source [D][B] -
					    source [A][A] * source [C][C] * source [D][B] -
					    source [A][B] * source [C][A] * source [D][C] +
					    source [A][A] * source [C][B] * source [D][C];

		result [D][C] = source [A][C] * source [B][B] * source [D][A] -
			            source [A][B] * source [B][C] * source [D][A] -
					    source [A][C] * source [B][A] * source [D][B] +
					    source [A][A] * source [B][C] * source [D][B] +
					    source [A][B] * source [B][A] * source [D][C] -
					    source [A][A] * source [B][B] * source [D][C];

		result [D][D] = source [A][B] * source [B][C] * source [C][A] -
			            source [A][C] * source [B][B] * source [C][A] +
					    source [A][C] * source [B][A] * source [C][B] -
					    source [A][A] * source [B][C] * source [C][B] -
					    source [A][B] * source [B][A] * source [C][C] +
					    source [A][A] * source [B][B] * source [C][C];

		return result;
	}

	Matrix4D Inverse ( const Matrix4D& source )
	{
		return Adjugate ( source ) / Determinant ( source );
	}
}