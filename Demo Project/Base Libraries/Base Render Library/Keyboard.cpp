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

#include "Keyboard.h"

#include <GL/glfw.h>

namespace Render
{
	//---------------------------------------- Constructor ----------------------------------------
	
	Keyboard :: Keyboard ( float step, bool local )
	{
		Step = step;

		Local = local;
	}

	//--------------------------------------- Event Handler ---------------------------------------

	void Keyboard :: StateChange ( int key, int state )
	{
		switch ( key )
		{
			case 'W': State.KeyW = state > 0; break;

			case 'S': State.KeyS = state > 0; break;

			case 'A': State.KeyA = state > 0; break;

			case 'D': State.KeyD = state > 0; break;

			case 'X': State.KeyX = state > 0; break;

			case 'Z': State.KeyZ = state > 0; break;

			case GLFW_KEY_LEFT: State.ArrowL = state > 0; break;

			case GLFW_KEY_RIGHT: State.ArrowR = state > 0; break;

			case GLFW_KEY_UP: State.ArrowU = state > 0; break;

			case GLFW_KEY_DOWN: State.ArrowD = state > 0; break;
		}
	}

	//---------------------- Applying new Position and Orientation to Camera ----------------------

	void Keyboard :: Apply ( Camera * camera )
	{
		//--------------------------------------------------------------------------

		if ( State.KeyW ) camera->MoveStraight ( Step );

		if ( State.KeyS ) camera->MoveStraight ( -Step );

		if ( State.KeyA ) camera->MoveHorizontal ( Step );

		if ( State.KeyD ) camera->MoveHorizontal ( -Step );

		if ( State.KeyX ) camera->MoveVertical ( Step );

		if ( State.KeyZ ) camera->MoveVertical ( -Step );

		//--------------------------------------------------------------------------
/*
		if ( Local )
		{
			if ( State.ArrowU ) camera->RotateLocal ( Step, Vector3D :: AxisX );

			if ( State.ArrowD ) camera->RotateLocal ( -Step, Vector3D :: AxisX );

			if ( State.ArrowL ) camera->RotateLocal ( Step, Vector3D :: AxisY );

			if ( State.ArrowR ) camera->RotateLocal ( -Step, Vector3D :: AxisY );
		}
		else
		{
			if ( State.ArrowU ) camera->RotateWorld ( Step, Vector3D :: AxisY );

			if ( State.ArrowD ) camera->RotateWorld ( -Step, Vector3D :: AxisY );

			if ( State.ArrowL ) camera->RotateWorld ( Step, Vector3D :: AxisZ );

			if ( State.ArrowR ) camera->RotateWorld ( -Step, Vector3D :: AxisZ );
		}
		*/
	}
}