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
#include "FilterMode.h"

namespace Render
{
	//-------------------------------------- Public Constants -------------------------------------
	
	const FilterMode FilterMode :: Nearest ( GL_NEAREST, GL_NEAREST );
	
	const FilterMode FilterMode :: Linear ( GL_LINEAR, GL_LINEAR );

	//---------------------------------------- Constructor ----------------------------------------

	FilterMode :: FilterMode ( int minification, int magnification )
	{
		Minification = minification;

		Magnification =  magnification;
	}

	//---------------------------------------- Setup Mode -----------------------------------------
			
	void FilterMode :: Setup ( int target )
	{
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, Minification );
				
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, Magnification );
	}
}