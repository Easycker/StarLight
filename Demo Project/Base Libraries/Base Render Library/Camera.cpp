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

#include "Camera.h"

namespace Render
{
	//---------------------------------------- Constructor ----------------------------------------

	Camera :: Camera ( const Vector3D& position, const Vector3D& orientation )
	{
		Position = position;

		Orientation = orientation;

		Update ( );		
	}

	//------------------------- Updating Axes of Camera Coordinate System -------------------------
			
	void Camera :: Update ( void )
	{
		Matrix3D rotation = Matrix3D :: Rotate ( Orientation );

		Side = Normalize ( rotation * Vector3D :: AxisX );

		Up = Normalize ( rotation * Vector3D :: AxisY );

		View = Normalize ( rotation * Vector3D :: AxisZ );
	}
	
	//------------------------- Moving and Rotating Camera in World Space -------------------------

	void Camera :: MoveStraight ( float delta )
	{
		Position += delta * View;
	}

	void Camera :: MoveHorizontal ( float delta )
	{
		Position += delta * Side;
	}

	void Camera :: MoveVertical ( float delta )
	{
		Position += delta * Up;
	}
	
	void Camera :: Rotate ( const Vector3D& delta )
	{
		Orientation += delta;

		Update ( );
	}

	//-------------------------- Applying Settings to OpenGL and Shaders --------------------------

	void Camera :: Setup ( void )
	{
		glViewport ( 0, 0, Width, Height );

		//-------------------------------------------------------------------------------

        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

        gluPerspective ( Degrees ( FieldOfView ), Aspect, NearPlane, FarPlane );
        

		//-------------------------------------------------------------------------------

		glMatrixMode ( GL_MODELVIEW );

		glLoadIdentity ( );

		gluLookAt ( Position.X,
			        Position.Y,
					Position.Z,
			        Position.X + View.X,
					Position.Y + View.Y,
					Position.Z + View.Z,
			        Up.X,
					Up.Y,
					Up.Z );
	}

	void Camera :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetUniformVector ( "Camera.Position", Position );
        	
		manager->SetUniformVector ( "Camera.Side", Side );
        	
		manager->SetUniformVector ( "Camera.Up", Up );
        	
		manager->SetUniformVector ( "Camera.View", View );

		manager->SetUniformVector ( "Camera.Scale", GetScreenScale ( ) );
	}

	//------------------------------- Setting Viewport and Frustum --------------------------------
	
	void Camera :: SetViewport ( unsigned width, unsigned height )
	{
		Width = width;

		Height = height;

		Aspect = Width / ( float ) Height;
	}

	void Camera :: SetFrustum ( float fieldOfView, float nearPlane, float farPlane )
	{
		FieldOfView = fieldOfView;

		NearPlane = nearPlane;

		FarPlane = farPlane;
	}

	//------------------------------------- Getting Settings --------------------------------------
	
	Vector2D Camera :: GetScreenScale ( void )
	{
		return Vector2D ( Aspect * tanf ( FieldOfView / 2.0F ), tanf ( FieldOfView / 2.0F ) );
	}

	Vector2D Camera :: GetPixelSize ( void )
	{
		return Vector2D ( 2.0F / Width, 2.0F / Height );
	}
}