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

#include "Texture1D.h"

#include <iostream>

using namespace std;

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Texture1D :: Texture1D ( unsigned unit )
	{
		Init ( GL_TEXTURE_1D, NULL, unit );
	}
				
	Texture1D :: Texture1D ( TextureData1D * data, unsigned unit )
	{
		Init ( GL_TEXTURE_1D, data, unit );
	}
	
	Texture1D :: ~Texture1D ( void )
	{
		delete Data;

		glDeleteTextures ( 1, &Handle );
	}

	//--------------------------------------- Texture Init ----------------------------------------

	void Texture1D :: Init ( unsigned target, TextureData1D * data, unsigned unit )
	{
		Target = target;

		Data = data;

		Unit = unit;

		glGenTextures ( 1, &Handle );
	}

	//------------------------------------ Texture Management -------------------------------------
			
	void Texture1D :: Setup ( void )
	{
		cout << "Setup texture [ \"" << Handle << "\" @ " << Unit << " ]" << endl;
	
		if ( NULL != Data )
		{
			Bind ( );

			WrapMode.Setup ( Target );

			FilterMode.Setup ( Target );
			
			Data->Upload ( Target );
		}
		else
		{
			cout << "ERROR: Texture data is empty" << endl;
		}			                
	}

	void Texture1D :: Update ( void )
	{
		if ( NULL != Data )
		{
			Bind ( );
			
			Data->Upload ( Target );
		}
		else
		{
			cout << "ERROR: Texture data is empty" << endl;
		}
	}
	
	void Texture1D :: Bind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, Handle );
	}
	
	void Texture1D :: Unbind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, 0 );			
	}
}