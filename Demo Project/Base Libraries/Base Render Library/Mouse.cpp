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

#include "Mouse.h"

namespace Render
{
	//---------------------------------------- Constructor ----------------------------------------
	
	Mouse :: Mouse ( float step, bool local )
	{
		Step = step;

		Local = local;
	}

	//-------------------------------------- Event Handlers ---------------------------------------
	
	void Mouse :: StateChange ( int button, int state )
	{
		glfwGetMousePos ( &LocationX, &LocationY );
		
		Active = state > 0;
	}
	
	void Mouse :: MouseMove ( int x, int y )
	{
		if ( Active )
		{
			if ( LocationX != x )
			{
				DeltaX += x - LocationX;

				LocationX = x;
			}
			
			if ( LocationY != y )
			{
				DeltaY += y - LocationY;

				LocationY = y;
			}	
		}
	}

	//---------------------- Applying new Position and Orientation to Camera ----------------------

	void Mouse :: Apply ( Camera * camera )
	{
		if ( DeltaX != 0 )
		{
			camera->Rotate ( DeltaX * Step * Vector3D :: AxisY );

			DeltaX = 0;
		}
		
		if ( DeltaY != 0 )
		{
			camera->Rotate ( -DeltaY * Step * Vector3D :: AxisX );
			
			DeltaY = 0;
		}
	}
}