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

#include "Util.h"

namespace Math
{
    float Degrees ( float radian )
	{
		return 180.0F * radian / ONEPI;
	}
    	
	float Radians ( float degree )
	{
		return ONEPI * degree / 180.0F;
	}  

	float Sign ( float source )
	{
		return source < 0.0F ? -1.0F : ( source > 0.0F ? 1.0F : 0.0F );		
	}

	float Fract ( float source )
	{
		return source - floorf ( source );
	}
	 
	float Mod ( float left, float right )
	{
		return left - right * floorf ( left / right );
	}

	float Clamp ( float source, float min, float max )
	{
		return source > min ? ( source < max ? source : max ) : min;
	}
	        
	float Mix ( float left, float right, float value )
	{
		return left * ( 1.0F - value ) + right * value;
	}
	     	
	float Step ( float source, float value )
	{
		return source < value ? 0.0F : 1.0F;
	}

	float Smooth ( float source, float left, float right )        	
	{
		float result = Clamp ( ( source - left ) / ( right - left ), 0.0F, 1.0F );

		return result * result * ( 3.0F - 2.0F * result );
	} 
}