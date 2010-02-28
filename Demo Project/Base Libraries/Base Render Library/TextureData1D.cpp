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

#include "TextureData1D.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	TextureData1D :: TextureData1D ( int width, int components )
	{
		Width = width;

		Components = components;

		Pixels = new float [Width * Components];

		memset ( Pixels, 0, Width * Components );
	}
			
	TextureData1D :: ~TextureData1D ( void )
	{
		delete [] Pixels;
	}

	//---------------------------------------- Data Upload ----------------------------------------

	void TextureData1D :: Upload ( int target )
	{
		glTexImage1D ( target,
			           0,
					   GetInternalFormat ( ),
					   Width,
					   0,
					   GetPixelFormat ( ),
					   GetType ( ),
					   Pixels );
	}
	
	//--------------------------------------- Texture Format --------------------------------------
	
	int TextureData1D :: GetPixelFormat ( void )
	{
		switch ( Components )
		{
			case 1:
				return GL_ALPHA;

			case 3:
				return GL_RGB;
					
			case 4:
				return GL_RGBA;
					
			default:
				return -1;
		}
	}

	int TextureData1D :: GetInternalFormat ( void )
	{
		switch ( Components )
		{
			case 1:
				return GL_ALPHA32F_ARB;

			case 3:
				return GL_RGB32F_ARB;
			
			case 4:
				return GL_RGBA32F_ARB;
			
			default:
				return -1;
		}
	}
}