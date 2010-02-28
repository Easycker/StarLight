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

#include "RenderBuffer.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	RenderBuffer :: RenderBuffer ( unsigned width,
		                           unsigned height,
								   unsigned attachment,
								   unsigned internalFormat )
	{
		Width = width;
		
		Height = height;
		
		Attachment = attachment;
		
		InternalFormat = internalFormat;
		
		glGenRenderbuffers ( 1, &Handle );
	}

	RenderBuffer :: ~RenderBuffer ( void )
	{
		glDeleteRenderbuffers ( 1, &Handle );
	}

	//------------------------------------- Buffer Management -------------------------------------
			
	void RenderBuffer :: Setup ( void )
	{
		Bind ( );
		
		glRenderbufferStorage ( Target, InternalFormat, Width, Height );	
	}

	void RenderBuffer :: Bind ( void )
	{
		glBindRenderbuffer ( Target, Handle );
	}
	
	void RenderBuffer :: Unbind ( void )
	{
		glBindRenderbuffer ( Target, 0 );
	}
}