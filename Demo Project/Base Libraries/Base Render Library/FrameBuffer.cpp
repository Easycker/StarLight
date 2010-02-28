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

#include "FrameBuffer.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	FrameBuffer :: FrameBuffer ( void )
	{
		glGenFramebuffers ( 1, &Handle );
	}

	FrameBuffer :: ~FrameBuffer ( void )
	{
		glDeleteFramebuffers ( 1, &Handle );
	}

	//------------------------------------- Buffer Management -------------------------------------
			
	void FrameBuffer :: Setup ( void )
	{
		if ( Handle == 0 )
		{
			return;
		}

		//--------------------------------------------------------------------------

		int current = 0;

        glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &current );
		
		if ( current != Handle )
		{
			glBindFramebuffer ( GL_FRAMEBUFFER, Handle );
		}

		//--------------------------------------------------------------------------
		
		{
			unsigned * buffers = new unsigned [ColorBuffers.size ( )];
            	
			for ( unsigned index = 0; index < ColorBuffers.size ( ); index++ )
			{
				glFramebufferTexture2D ( Target,
					                     GL_COLOR_ATTACHMENT0 + index,
										 ColorBuffers [index]->GetTarget ( ),
										 ColorBuffers [index]->GetHandle ( ),
										 0 );

				buffers [index] = GL_COLOR_ATTACHMENT0 + index;
			}

			glDrawBuffers ( ColorBuffers.size ( ), buffers );

			delete [] buffers;
		}

		//--------------------------------------------------------------------------
		
		{
			for ( unsigned index = 0; index < RenderBuffers.size ( ); index++ )
			{
				glFramebufferRenderbuffer ( Target,
					                        RenderBuffers [index]->Attachment,
											RenderBuffers [index]->Target,
											RenderBuffers [index]->GetHandle ( ) );
			}
		}

		//--------------------------------------------------------------------------
		
		if ( current != Handle )
		{
			glBindFramebuffer ( Target, current );
		}
	}
	
	void FrameBuffer :: Bind ( void )
	{
		glBindFramebuffer ( Target, Handle );
	}
	
	void FrameBuffer :: Unbind ( void )
	{
		glBindFramebuffer ( Target, 0 );
	}
	
	void FrameBuffer :: FetchOutcome ( void )
	{
		for ( unsigned index = 0; index < ColorBuffers.size ( ); index++ )
		{
			glReadBuffer ( GL_COLOR_ATTACHMENT0 + index );
				
			glReadPixels ( 0,
				           0,
				           ColorBuffers [index]->Data->GetWidth ( ),
						   ColorBuffers [index]->Data->GetHeight ( ),
				           ColorBuffers [index]->Data->GetPixelFormat ( ),
						   ColorBuffers [index]->Data->GetType ( ),
						   * ( ColorBuffers [index]->Data ) );
		}	
	}
}